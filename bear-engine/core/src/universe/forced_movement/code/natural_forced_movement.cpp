/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::natural_forced_movement class.
 * \author Sébastien Angibaud
 */
#include "universe/forced_movement/natural_forced_movement.hpp"

#include "universe/physical_item.hpp"
#include "universe/world.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param duration The duration of the movement.
 */
bear::universe::natural_forced_movement::natural_forced_movement
( time_type duration )
  : m_duration(duration), m_remaining_time(m_duration)
{

} // natural_forced_movement::natural_forced_movement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instantiates a copy of this movement.
 */
bear::universe::base_forced_movement*
bear::universe::natural_forced_movement::clone() const
{
  return new natural_forced_movement(*this);
} // natural_forced_movement::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the total duration of the movement.
 * \param d The total duration of the movement.
 */
void bear::universe::natural_forced_movement::set_total_time( time_type d )
{
  m_duration = d;
} // natural_forced_movement::set_total_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the movement is finished.
 */
bool bear::universe::natural_forced_movement::is_finished() const
{
  return m_remaining_time == 0;
} // natural_forced_movement::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initializes the movement.
 */
void bear::universe::natural_forced_movement::do_init()
{
  m_remaining_time = m_duration;
} // natural_forced_movement::do_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Updates the position of the moving item.
 * \param elapsed_time Elapsed time since the last call.
 * \return The remaining time, if the movement is finished.
 */
bear::universe::time_type
bear::universe::natural_forced_movement::do_next_position
( time_type elapsed_time )
{
  time_type remaining_time(0);

  if ( elapsed_time > m_remaining_time )
    {
      remaining_time = elapsed_time - m_remaining_time;
      elapsed_time = m_remaining_time;
    }

  update_position( elapsed_time );

  m_remaining_time -= elapsed_time;

  return remaining_time;
} // natural_forced_movement::do_next_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Updates the position, the speed and the acceleration of the item.
 * \param elapsed_time Elasped time since the last progress.
 */
void bear::universe::natural_forced_movement::update_position
( time_type elapsed_time )
{
  physical_item& item( get_item() );
  const world *const w( item.has_owner() ? &item.get_owner() : NULL );

  force_type force( item.get_force() );

  if ( w != NULL )
    {
      force += w->get_average_force( item.get_bounding_box() );

      if ( (item.get_density() != 0)
           && ( item.get_mass() != std::numeric_limits<double>::infinity() )  )
        force -= w->get_gravity() * item.get_mass()
          * w->get_average_density( item.get_bounding_box() )
          / item.get_density();
    }

  force_type a( force / item.get_mass() );
  double f = item.get_friction() * item.get_contact_friction();

  if ( (w != NULL)
       && ( item.get_mass() != std::numeric_limits<double>::infinity() ) )
    {
      a += w->get_gravity();
      f *= w->get_average_friction( item.get_bounding_box() );
    }

  const speed_type speed( f * ( a * elapsed_time + item.get_speed() ) );
  set_moving_item_position( get_moving_item_position() + speed * elapsed_time );

  const double angular_speed( item.get_angular_speed() * elapsed_time * f );
  item.set_system_angle( item.get_system_angle() + angular_speed );

  item.set_acceleration( a );
  item.set_internal_force( force_type(0, 0) );
  item.set_external_force( force_type(0, 0) );
} // natural_forced_movement::update_position()
