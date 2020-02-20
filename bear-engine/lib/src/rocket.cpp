/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file rocket.cpp
 * \brief Implementation of the rocket class.
 * \author Sebastien Angibaud
 */
#include "bear/generic_items/rocket.hpp"
#include "bear/generic_items/path_trace.hpp"

BASE_ITEM_EXPORT( rocket, bear )

const bear::universe::force_type bear::rocket::s_force = 
  bear::universe::force_type(0,50000);

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::rocket::rocket()
: m_date(0), 
  m_min_force_factor(1.0), m_max_force_factor(1.0),
  m_initial_angle(0),
  m_min_angle(0), m_max_angle(0),
  m_min_nb_rockets(1), m_max_nb_rockets(1), 
  m_min_nb_explose_rockets(0), m_max_nb_explose_rockets(0), 
  m_min_explosion_date(std::numeric_limits<universe::time_type>::max()), 
  m_max_explosion_date(std::numeric_limits<universe::time_type>::max()),
  m_trace_length(0),
  m_trace_color(claw::graphic::black_pixel),
  m_min_fade_out_speed(std::numeric_limits<universe::time_type>::max()),
  m_max_fade_out_speed(std::numeric_limits<universe::time_type>::max())  
{
  set_system_angle_as_visual_angle(true);
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
  set_kill_when_leaving(true);  
  set_density(0.0015);
  set_mass(1);
} // rocket::bear::rocket()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy contructor.
 */
bear::rocket::rocket( const rocket& that )
  : super(that),
    m_date(0), 
    m_min_force_factor(that.m_min_force_factor), 
    m_max_force_factor(that.m_max_force_factor),
    m_initial_angle(that.m_initial_angle),
    m_min_angle(that.m_min_angle), m_max_angle(that.m_max_angle),
    m_min_nb_rockets(that.m_min_nb_rockets),
    m_max_nb_rockets(that.m_max_nb_rockets),
    m_min_nb_explose_rockets(that.m_min_nb_explose_rockets), 
    m_max_nb_explose_rockets(that.m_max_nb_explose_rockets), 
    m_min_explosion_date(that.m_min_explosion_date), 
    m_max_explosion_date(that.m_max_explosion_date),
    m_trace_length(that.m_trace_length),
    m_trace_color(that.m_trace_color),
    m_min_fade_out_speed(that.m_min_fade_out_speed),
    m_max_fade_out_speed(that.m_max_fade_out_speed)    
{
  for ( unsigned int i = 0; i != that.m_rockets.size(); ++i )
    m_rockets.push_back( that.m_rockets[i]->clone() );
} // rocket::rocket()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::rocket::~rocket()
{
  for ( unsigned int i = 0; i != m_rockets.size(); ++i )
    delete m_rockets[i];
} // rocket::~rocket()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do post creation actions.
 */
void bear::rocket::on_enters_layer()
{
  super::on_enters_layer();
  
  m_nb_rockets = get_rand<unsigned int>(m_min_nb_rockets, m_max_nb_rockets + 1);
 
  if ( m_nb_rockets <= 1 )
    throw_rocket();
  else
    duplicate_rocket();
} // rocket::on_enters_layer()

/*---------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::rocket::progress( universe::time_type elapsed_time )
{
  super::progress( elapsed_time );
  
  get_rendering_attributes().set_angle
    ( std::atan2( get_speed().y, get_speed().x) );

  if ( m_date < m_explosion_date )
    {
      m_date += elapsed_time;

      if ( m_date >= m_explosion_date )
        explose();
    }
  else
    {
      m_date += elapsed_time;
      
      double opacity = get_rendering_attributes().get_opacity();
      opacity -= m_fade_out_speed * elapsed_time;
      get_rendering_attributes().set_opacity(opacity);

      if (opacity <= 0)
        kill();
    }
} // rocket::progress()

/*---------------------------------------------------------------------------*/
/**
 * \brief Set the force factor to applied.
 * \param min_force The minimal force factor.
 * \param max_force The maximal force factor.
 */
void bear::rocket::set_force_factor
( double min_force_factor, double max_force_factor)
{
  m_min_force_factor = min_force_factor;
  m_max_force_factor = max_force_factor;
} // rocket::set_force_factor()

/*---------------------------------------------------------------------------*/
/**
 * \brief Set the intial angle.
 * \param angle The initial angle.
 */
void bear::rocket::set_initial_angle( double angle)
{
  m_initial_angle = angle;
} // rocket::set_intial_angle()

/*---------------------------------------------------------------------------*/
/**
 * \brief Set the angle to give.
 * \param min_angle The minimal angle.
 * \param max_angle The maximal angle.
 */
void bear::rocket::set_angle( double min_angle, double max_angle)
{
  m_min_angle = min_angle;
  m_max_angle = max_angle;
} // rocket::set_angle()

/*---------------------------------------------------------------------------*/
/**
 * \brief Set the number of current rockets to create at the beginning.
 * \param min_nb The minimal number of rockets.
 * \param max_nb The maximal number of rockets.
 */
void bear::rocket::set_rocket_count( unsigned int min_nb, unsigned int max_nb)
{
  m_min_nb_rockets = min_nb;
  m_max_nb_rockets = max_nb;
} // rocket::set_rocket_count()

/*---------------------------------------------------------------------------*/
/**
 * \brief Set the number of rockets to create when the item exploses.
 * \param min_nb The minimal number of rockets.
 * \param max_nb The maximal number of rockets.
 */
void bear::rocket::set_explosion_rocket_count
( unsigned int min_nb, unsigned int max_nb)
{
  m_min_nb_explose_rockets = min_nb;
  m_max_nb_explose_rockets = max_nb;
} // rocket::set_explosion_rocket_count()

/*---------------------------------------------------------------------------*/
/**
 * \brief Set the date of the explosion.
 * \param min_date The minimal date.
 * \param max_date The maximal date.
 */
void bear::rocket::set_explosion_date
( universe::time_type min_date, universe::time_type max_date )
{
  m_min_explosion_date = min_date;
  m_max_explosion_date = max_date;
} // rocket::set_explosion_date()

/*---------------------------------------------------------------------------*/
/**
 * \brief Set the trace.
 * \param length The minimal duration.
 * \param color The color of the trace.
 * \param opacity The opacity of the trace
 */
void bear::rocket::set_trace
( universe::time_type length, const visual::color_type& color, double opacity )
{  
  m_trace_color = color;
  m_trace_color.set_opacity(opacity);

  m_trace_length = length;
} // rocket::set_trace()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the speed of the fade out when the rocket exploses.
 * \param min_s Minimum of units of opacity lost per second.
 * \param max_s Maximum of units of opacity lost per second.
 */
void bear::rocket::set_fade_out_speed( double min_s, double max_s )
{
  m_min_fade_out_speed = min_s;
  m_max_fade_out_speed = max_s;
} // rocket::set_fade_out_speed()

/*---------------------------------------------------------------------------*/
/**
 * \brief Add a possible rocket to create.
 * \param r The rocket to create when the item explose.
 */
void bear::rocket::add_rocket( const rocket& r)
{
  m_rockets.push_back(r.clone());
} // rocket::add_rocket()

/*---------------------------------------------------------------------------*/
/**
 * \brief Sets the name of the sound to play when the rocket exploses.
 * \param name The name of the sound.
 */
void bear::rocket::set_explosion_sound_name( std::string name )
{
  m_explosion_sound_name = name;
} // rocket::set_explosion_sound_name()

/*---------------------------------------------------------------------------*/
/**
 * \brief The item exploses.
 */
void bear::rocket::explose()
{
  if ( ! m_rockets.empty() )
    {
      for ( unsigned int i = 0; i != m_nb_explose_rockets; ++i )
        create_rocket( (double)i * 6.28 / m_nb_explose_rockets );
      
      play_explosion_sound();

      kill();
    }
} // rocket::explose()

/*---------------------------------------------------------------------------*/
/**
 * \brief Duplicate the current rocket.
 */
void bear::rocket::duplicate_rocket()
{
  for ( unsigned int i = 0; i != m_nb_rockets; ++i )
    {
      rocket* r = this->clone();
      r->set_rocket_count(1, 1);
      
      new_item(*r);
    }
  
  kill();
} // rocket::duplicate_rocket()

/*---------------------------------------------------------------------------*/
/**
 * \brief Create a rocket.
 * \paral initial_angle The initial angle of the rocket.
 */
void bear::rocket::create_rocket(double initial_angle)
{
  unsigned int alea = get_rand<unsigned int>(0, m_rockets.size());
 
  rocket* r = m_rockets[alea]->clone();
  r->set_initial_angle(initial_angle);
  r->set_z_position( get_z_position() );
  r->set_center_of_mass(get_center_of_mass());
  r->set_kill_when_leaving( get_kill_when_leaving() );

  new_item(*r);
} // rocket::create_rocket()

/*---------------------------------------------------------------------------*/
/**
 * \brief Create trace.
 */
void bear::rocket::create_trace()
{
  path_trace* trace = new path_trace(*this);

  trace->set_fill_color(m_trace_color);
  trace->set_fade_out_speed( 2.0 );
  trace->set_tail_ratio( 0 );
  trace->set_length(m_trace_length);

  new_item(*trace);
} // rocket::create_trace()

/*---------------------------------------------------------------------------*/
/**
 * \brief Play a sound for the explosion.
 */
void bear::rocket::play_explosion_sound()
{
  if ( get_level_globals().sound_exists( m_explosion_sound_name ) )
    get_level_globals().play_sound
      ( m_explosion_sound_name,
        audio::sound_effect( get_center_of_mass() ) );
} // rocket::play_explosion_sound()

/*---------------------------------------------------------------------------*/
/**
 * \brief Throw the rocket.
 */
void bear::rocket::throw_rocket()
{
  m_nb_explose_rockets = 
    get_rand<unsigned int>
    (m_min_nb_explose_rockets, m_max_nb_explose_rockets + 1);
  m_explosion_date = 
    get_rand<universe::time_type>(m_min_explosion_date, m_max_explosion_date);
  m_fade_out_speed = 
    get_rand<universe::time_type>(m_min_fade_out_speed, m_max_fade_out_speed);
  
  set_system_angle
    ( m_initial_angle + get_rand<double>(m_min_angle, m_max_angle) );
  set_internal_force
    ( s_force * get_rand<double>(m_min_force_factor, m_max_force_factor) );
  
  if ( m_trace_length != 0 )
    create_trace();
} // rocket::throw_rocket()

/*---------------------------------------------------------------------------*/
/**
 * \brief Get a random number between two values.
 * \param min The minimal value.
 * \param max The maximal value.
 */
template<class T> 
T bear::rocket::get_rand(T min, T max) const
{
   return (T)( (double)min + (double)rand() / RAND_MAX * (double)(max - min));
} // rocket::get_rand()
