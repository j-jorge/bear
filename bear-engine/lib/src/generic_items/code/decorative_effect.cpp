/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::decorative_effect class.
 * \author Julien Jorge
 */
#include "generic_items/decorative_effect.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( decorative_effect, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::decorative_effect::decorative_effect()
: m_duration(0), m_elapsed_time(0), m_loop_back(false), 
  m_size_factor_init(1), m_size_factor_end(1),
  m_angle_offset_init(1),
  m_angle_offset_end(1), m_item(NULL), m_same_lifespan(false),
  m_restore_at_end(false)
{
  set_artificial(true);
  set_phantom(true);
  set_can_move_items(false);
  m_color_init.set(1,1,1,1);
  m_color_end.set(1,1,1,1);
} // decorative_effect::decorative_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item.
 */
void bear::decorative_effect::build()
{
  super::build();

  if ( m_item == (engine::with_rendering_attributes*)NULL )
    kill();
  else
    {
      m_rendering_attributes = m_item->get_rendering_attributes();

      apply_effect();

      // follow the item
      set_center_of_mass( m_item.get_item()->get_center_of_mass() );
    }
} // decorative_effect::build()

/*---------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::decorative_effect::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  m_elapsed_time += elapsed_time;

  if ( m_item == (engine::with_rendering_attributes*)NULL )
    kill();
  else if ( m_elapsed_time >= m_duration )
    {
      kill();

      if (m_same_lifespan)
        m_item.get_item()->kill();
      else if (m_restore_at_end)
        m_item->set_rendering_attributes( m_rendering_attributes );
    }
  else
    {
      apply_effect();

      // follow the item
      set_center_of_mass( m_item.get_item()->get_center_of_mass() );
    }
} // decorative_effect::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::decorative_effect::set_bool_field( const std::string& name, bool value )
{
  bool ok = true;

  if (name == "decorative_effect.same_lifespan")
    m_same_lifespan = value;
  else if (name == "decorative_effect.restore_at_end")
    m_restore_at_end = value;
  else if (name == "decorative_effect.loop_back")
    m_loop_back = value;
  else
    ok = super::set_bool_field(name, value);

  return ok;
} // decorative_effect::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c base_item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::decorative_effect::set_item_field
( const std::string& name, base_item* value )
{
  bool ok = true;

  if (name == "decorative_effect.item")
    {
      m_item = value;
      set_center_of_mass( value->get_center_of_mass() );
    }
  else
    ok = super::set_bool_field(name, value);

  return ok;
} // decorative_effect::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::decorative_effect::set_real_field( const std::string& name, double value )
{
  bool ok = true;

  if (name == "decorative_effect.duration")
    m_duration = value;
  else if (name == "decorative_effect.size_factor.init")
    m_size_factor_init = value;
  else if (name == "decorative_effect.size_factor.end")
    m_size_factor_end = value;
  else if (name == "decorative_effect.angle_offset.init")
    m_angle_offset_init = value;
  else if (name == "decorative_effect.angle_offset.end")
    m_angle_offset_end = value;
  else
    ok = super::set_real_field(name, value);

  return ok;
} // decorative_effect::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c color.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::decorative_effect::set_color_field
( const std::string& name, visual::color value )
{
  bool ok = true;

  if (name == "decorative_effect.color.init")
    m_color_init = value;
  else if (name == "decorative_effect.color.end")
    m_color_end = value;
  else
    ok = super::set_color_field(name, value);

  return ok;
} // decorative_effect::set_color_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the item on which the effect is applyed.
 * \param d The total duration.
 * \param same_lifespan Tell if \a item must be killed with *this.
 * \param restore Tell if the state of the item must be restored at the end.
 * \param loop_back Indicates if the effect has a loop back.
 */
void bear::decorative_effect::set_item
( engine::base_item* item, bool same_lifespan, bool restore, bool loop_back )
{
  m_item = item;
  m_same_lifespan = same_lifespan;
  m_restore_at_end = restore;
  m_loop_back = loop_back;

  if ( item != NULL )
    set_center_of_mass( item->get_center_of_mass() );
} // decorative_effect::set_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the duration of the effect.
 * \param d The total duration.
 */
void bear::decorative_effect::set_duration(universe::time_type d)
{
  m_duration = d;
} // decorative_effect::set_duration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the duration of the effect.
 */
bear::universe::time_type bear::decorative_effect::get_duration() const
{
  return m_duration;
} // decorative_effect::get_duration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the initial factor applied to the size.
 * \param f The factor to apply.
 */
void bear::decorative_effect::set_size_factor_init(double f)
{
  m_size_factor_init = f;
} // decorative_effect::set_size_factor_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the initial color.
 * \param c The color to apply.
 */
void bear::decorative_effect::set_color_init(const visual::color& c)
{
  m_color_init = c;
} // decorative_effect::set_color_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the initial factor applied to the angle.
 * \param f The factor to apply.
 */
void bear::decorative_effect::set_angle_offset_init(double f)
{
  m_angle_offset_init = f;
} // decorative_effect::set_angle_offset_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the final factor applied to the size.
 * \param f The factor to apply.
 */
void bear::decorative_effect::set_size_factor_end(double f)
{
  m_size_factor_end = f;
} // decorative_effect::set_size_factor_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the final clor.
 * \param c The color to apply.
 */
void bear::decorative_effect::set_color_end(const visual::color& c)
{
  m_color_end = c;
} // decorative_effect::set_color_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the final factor applied to the angle.
 * \param f The factor to apply.
 */
void bear::decorative_effect::set_angle_offset_end(double f)
{
  m_angle_offset_end = f;
} // decorative_effect::set_angle_offset_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the offsets applied to the size.
 * \param init The initial factor to apply.
 * \param end The final factor to apply.
 */
void bear::decorative_effect::set_size_factor(double init, double end)
{
  set_size_factor_init(init);
  set_size_factor_end(end);
} // decorative_effect::set_size_factor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the colors applied.
 * \param init_color The initial color to apply.
 * \param end_color The final color to apply.
 */
void bear::decorative_effect::set_color
(const visual::color& init_color, const visual::color& end_color)
{
  m_color_init = init_color;
  m_color_end = end_color;
} // decorative_effect::set_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the offsets applied to the angle.
 * \param init The initial factor to apply.
 * \param end The final factor to apply.
 */
void bear::decorative_effect::set_angle_offset(double init, double end)
{
  set_angle_offset_init(init);
  set_angle_offset_end(end);
} // decorative_effect::set_angle_offset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the effect to the item.
 */
void bear::decorative_effect::apply_effect() const
{
  double r = m_elapsed_time / m_duration;

  if ( m_loop_back )
    {
      r = r * 2.0;
      if ( r > 1.0 )
        r = 2.0 - r;
    }

  m_item->get_rendering_attributes().set_width
    ( m_rendering_attributes.width()
      * (m_size_factor_init + (m_size_factor_end - m_size_factor_init) * r) );
  
  m_item->get_rendering_attributes().set_height
    ( m_rendering_attributes.height()
      * (m_size_factor_init + (m_size_factor_end - m_size_factor_init) * r) );
  
  m_item->get_rendering_attributes().set_opacity
    ( m_rendering_attributes.get_opacity()
      * (m_color_init.get_opacity()
         + (m_color_end.get_opacity() - m_color_init.get_opacity()) * r) );
  
  m_item->get_rendering_attributes().set_angle
    ( m_rendering_attributes.get_angle()
      + (m_angle_offset_end - m_angle_offset_init) * r );

  m_item->get_rendering_attributes().set_red_intensity
    ( m_rendering_attributes.get_red_intensity()
      * ( m_color_init.get_red_intensity()
          + ( m_color_end.get_red_intensity() - 
              m_color_init.get_red_intensity() ) * r) );
  
  m_item->get_rendering_attributes().set_green_intensity
    ( m_rendering_attributes.get_green_intensity()
      * ( m_color_init.get_green_intensity()
          + ( m_color_end.get_green_intensity() - 
              m_color_init.get_green_intensity() ) * r) );
  
  m_item->get_rendering_attributes().set_blue_intensity
    ( m_rendering_attributes.get_blue_intensity()
      * (m_color_init.get_blue_intensity()
         + ( m_color_end.get_blue_intensity() - 
             m_color_init.get_blue_intensity() ) * r) );
} // decorative_effect::apply_effect()
