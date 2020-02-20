/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::decorated_item_with_toggle class.
 * \author Julien Jorge
 */

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type animation.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::decorated_item_with_toggle<Base>::set_animation_field
( const std::string& name, const visual::animation& value )
{
  bool ok = true;

  if (name == "decorated_item_with_toggle.visual_on")
    set_toggle_visual_on( value );
  else if ( name == "decorated_item_with_toggle.visual_off" )
    set_toggle_visual_off( value );
  else
    ok = super::set_animation_field(name, value);

  return ok;
} // decorated_item_with_toggle::set_animation_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprites representing the item.
 * \param visuals (out) The sprites of the item, and their positions.
 */
template<class Base>
void bear::engine::decorated_item_with_toggle<Base>::get_visual
( std::list<scene_visual>& visuals ) const
{
  super::get_visual(visuals);

  if ( this->is_on() )
    this->add_visual( m_visual_on, visuals );
  else
    this->add_visual( m_visual_off, visuals );
} // decorated_item_with_toggle::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the animation shown when the toggle is on.
 * \param anim The animation.
 */
template<class Base>
void bear::engine::decorated_item_with_toggle<Base>::set_toggle_visual_on
( const visual::animation& anim )
{
  m_visual_on = anim;
} // decorated_item_with_toggle::set_toggle_visual_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the animation shown when the toggle is off.
 * \param anim The animation.
 */
template<class Base>
void bear::engine::decorated_item_with_toggle<Base>::set_toggle_visual_off
( const visual::animation& anim )
{
  m_visual_off = anim;
} // decorated_item_with_toggle::set_toggle_visual_off()

/*----------------------------------------------------------------------------*/
/**
 * \brief This method is called when the toggle switches on.
 * \param activator The item that activates the toggle, if any.
 */
template<class Base>
void bear::engine::decorated_item_with_toggle<Base>::on_toggle_on
( base_item* activator )
{
  super::on_toggle_on(activator);
  m_visual_on.reset();
} // decorated_item_with_toggle::on_toggle_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief This method is called when the toggle switches off.
 * \param activator The item that activates the toggle, if any.
 */
template<class Base>
void bear::engine::decorated_item_with_toggle<Base>::on_toggle_off
( base_item* activator )
{
  super::on_toggle_off(activator);
  m_visual_off.reset();
} // decorated_item_with_toggle::on_toggle_off()

/*----------------------------------------------------------------------------*/
/**
 * \brief Progress the item as it is turned on.
 * \param elapsed_time The duration of the activity.
 */
template<class Base>
void bear::engine::decorated_item_with_toggle<Base>::progress_on
( universe::time_type elapsed_time )
{
  super::progress_on(elapsed_time);
  m_visual_on.next(elapsed_time);
} // decorated_item_with_toggle::progress_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Progress the item as it is turned off.
 * \param elapsed_time The duration of the activity.
 */
template<class Base>
void bear::engine::decorated_item_with_toggle<Base>::progress_off
( universe::time_type elapsed_time )
{
  super::progress_off(elapsed_time);
  m_visual_off.next(elapsed_time);
} // decorated_item_with_toggle::progress_off()
