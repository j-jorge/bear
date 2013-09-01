/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::mouse_detector class.
 * \author Sebastie Angibaud
 */
#include "generic_items/mouse_detector.hpp"
#include "engine/export.hpp"
#include "engine/game.hpp"
#include "engine/level.hpp"

BASE_ITEM_EXPORT( mouse_detector, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::mouse_detector::mouse_detector()
: m_toggle(NULL), 
  m_right_button(false), m_left_button(true), m_middle_button(false), 
  m_wheel_up(false), m_wheel_down(false), m_finger(true)
{
  set_artificial(true);
  set_phantom(true); 
} // mouse_detector::mouse_detector()

/*---------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 */
void bear::mouse_detector::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  progress_input_reader(elapsed_time);
} // mouse_detector::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::mouse_detector::set_bool_field
( const std::string& name, bool value )
{
  bool result = true;

  if (name == "mouse_detector.right_button")
    m_right_button = value;
  else if (name == "mouse_detector.left_button")
    m_left_button = value; 
  else if (name == "mouse_detector.middle_button")
    m_middle_button = value; 
  else if (name == "mouse_detector.wheel_up")
    m_wheel_up = value;
  else if (name == "mouse_detector.wheel_down")
    m_wheel_down = value;
  else if (name == "mouse_detector.finger")
    m_finger = value;
  else
    result = super::set_bool_field(name, value);

  return result;
} // mouse_detector::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::mouse_detector::set_item_field
( const std::string& name, bear::engine::base_item* value)
{
  bool ok = true;

  if (name == "mouse_detector.toggle")
    m_toggle = handle_type(value);
  else
    ok = super::set_item_field(name, value);

  return ok;
} // mouse_detector::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the player to stop the action associated with a mouse button.
 * \param button The code of the button.
 * \param pos The position of the cursor on the screen.
 */
bool bear::mouse_detector::mouse_released
( bear::input::mouse::mouse_code button,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  bool result = false;
  
  if ( check_mouse_position( button, pos ) )
    {
      result = true;

      if ( m_toggle != (physical_item*)NULL )
        m_toggle->toggle(this);
    }
  else
    result = false;

  return result;
} // mouse_detector::mouse_released

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the player to stop the action associated with a mouse button.
 * \param button The code of the button.
 * \param pos The position of the cursor on the screen.
 */
bool bear::mouse_detector::check_mouse_position
( bear::input::mouse::mouse_code button,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  return 
    ( get_bounding_box().includes( get_level().screen_to_level(pos) ) &&
      ( ( button == bear::input::mouse::mc_right_button && m_right_button ) ||
        ( button == bear::input::mouse::mc_left_button && m_left_button ) ||
        ( button == bear::input::mouse::mc_wheel_down && m_wheel_down ) ||
        ( button == bear::input::mouse::mc_wheel_up && m_wheel_up ) ||
        ( button == bear::input::mouse::mc_middle_button && m_middle_button)));
} // mouse_detector::check_mouse_position()


/*----------------------------------------------------------------------------*/
/**
 * \brief Process an event related to the finger. If the finger is down within
 *        the bounds of the item, the toggle is activated.
 * \param event The event to process.
 */
bool bear::mouse_detector::finger_action( const input::finger_event& event )
{
  bool result = false;
  
  if ( (event.get_type() == input::finger_event::finger_event_pressed)
       && get_bounding_box().includes
       ( get_level().screen_to_level( event.get_position() ) ) )
    {
      if ( m_toggle != (physical_item*)NULL )
        m_toggle->toggle(this);
    }

  return result;
} // mouse_detector::finger_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the items concerned by a progress/move of this one.
 * \param d (out) A list to which are added such items.
 */
void bear::mouse_detector::get_dependent_items
( std::list<bear::universe::physical_item*>& d ) const
{
  super::get_dependent_items(d);

  if ( m_toggle != (physical_item*)NULL )
      d.push_back(m_toggle.get_item());
} // mouse_detector::get_dependent_items()
