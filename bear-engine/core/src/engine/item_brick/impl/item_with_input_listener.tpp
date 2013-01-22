/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::with_input_reader.
 * \author Julien Jorge
 */

#include "engine/level.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Read inputs and apply them to the item.
 * \param elapsed_time Elapsed time since the last call.
 */
template<class Base>
void bear::engine::item_with_input_listener<Base>::progress_input_reader
( universe::time_type elapsed_time )
{
  m_elapsed_time = elapsed_time;
  m_input_status.read();
  m_input_status.scan_inputs( *this );
} // item_with_input_listener::progress_input_reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief A keyboard key is maintained.
 * \param elapsed_time How long the key has been pressed.
 * \param key The code of the key.
 */
template<class Base>
bool bear::engine::item_with_input_listener<Base>::key_maintained
( universe::time_type elapsed_time, const input::key_info& key )
{
  return false;
} // item_with_input_listener::key_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief A joystick button is maintained.
 * \param elapsed_time How long the button has been pressed.
 * \param button The code of the button.
 * \param joy_index The index of the joystick.
 */
template<class Base>
bool bear::engine::item_with_input_listener<Base>::button_maintained
( universe::time_type elapsed_time, input::joystick::joy_code button,
  unsigned int joy_index )
{
  return false;
} // item_with_input_listener::button_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief A mouse button is maintained.
 * \param elapsed_time How long the button has been pressed.
 * \param button The code of the button.
 * \param pos The position of the cursor on the screen.
 */
template<class Base>
bool bear::engine::item_with_input_listener<Base>::mouse_maintained
( universe::time_type elapsed_time, input::mouse::mouse_code button,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  const universe::position_type global
    ( this->get_level().screen_to_level( pos ) );

  if ( this->get_bounding_box().includes( global ) )
    return mouse_maintained_local
      ( elapsed_time, button, global - this->get_bottom_left() );
  else
    return false;
} // item_with_input_listener::mouse_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief A mouse button has been pressed on the item.
 * \param button The code of the button.
 * \param pos The position of the cursor on the item.
 */
template<class Base>
bool bear::engine::item_with_input_listener<Base>::mouse_pressed_local
( input::mouse::mouse_code button, const universe::position_type& pos )
{
  return false;
} // item_with_input_listener::mouse_pressed_local()

/*----------------------------------------------------------------------------*/
/**
 * \brief A mouse button has been maintained on the item.
 * \param elapsed_time How long the button has been pressed.
 * \param button The code of the button.
 * \param pos The position of the cursor on the item.
 */
template<class Base>
bool bear::engine::item_with_input_listener<Base>::mouse_maintained_local
( universe::time_type elapsed_time, input::mouse::mouse_code button,
  const universe::position_type& pos )
{
  return false;
} // item_with_input_listener::mouse_maintained_local()

/*----------------------------------------------------------------------------*/
/**
 * \brief A mouse button has been released on the item.
 * \param button The code of the button.
 * \param pos The position of the cursor on the item.
 */
template<class Base>
bool bear::engine::item_with_input_listener<Base>::mouse_released_local
( input::mouse::mouse_code button, const universe::position_type& pos )
{
  return false;
} // item_with_input_listener::mouse_released_local()

/*----------------------------------------------------------------------------*/
/**
 * \brief The mouse is moving over the item.
 * \param pos The position of the cursor on the item.
 */
template<class Base>
bool bear::engine::item_with_input_listener<Base>::mouse_move_local
( const universe::position_type& pos )
{
  return false;
} // item_with_input_listener::mouse_move_local()

/*----------------------------------------------------------------------------*/
/**
 * \brief A keyboard key is maintained.
 * \param key The code of the key.
 */
template<class Base>
bool bear::engine::item_with_input_listener<Base>::key_maintained
( const input::key_info& key )
{
  return key_maintained(m_elapsed_time, key);
} // item_with_input_listener::key_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief A joystick button is maintained.
 * \param button The code of the button.
 * \param joy_index The index of the joystick.
 */
template<class Base>
bool bear::engine::item_with_input_listener<Base>::button_maintained
( input::joystick::joy_code button, unsigned int joy_index )
{
  return button_maintained( m_elapsed_time, button, joy_index );
} // item_with_input_listener::button_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief A mouse button is pressed.
 * \param button The code of the button.
 * \param pos The position of the cursor on the screen.
 */
template<class Base>
bool bear::engine::item_with_input_listener<Base>::mouse_pressed
( input::mouse::mouse_code button,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  const universe::position_type global
    ( this->get_level().screen_to_level( pos ) );

  if ( this->get_bounding_box().includes( global ) )
    return mouse_pressed_local( button, global - this->get_bottom_left() );
  else
    return input_listener::mouse_pressed( button, pos );
} // item_with_input_listener::mouse_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief A mouse button is maintained.
 * \param button The code of the button.
 * \param pos The position of the cursor on the screen.
 */
template<class Base>
bool bear::engine::item_with_input_listener<Base>::mouse_maintained
( input::mouse::mouse_code button,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  return mouse_maintained( m_elapsed_time, button, pos );
} // item_with_input_listener::mouse_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief A mouse button is released.
 * \param button The code of the button.
 * \param pos The position of the cursor on the screen.
 */
template<class Base>
bool bear::engine::item_with_input_listener<Base>::mouse_released
( input::mouse::mouse_code button,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  const universe::position_type global
    ( this->get_level().screen_to_level( pos ) );

  if ( this->get_bounding_box().includes( global ) )
    return mouse_released_local( button, global - this->get_bottom_left() );
  else
    return input_listener::mouse_released( button, pos );
} // item_with_input_listener::mouse_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief The mouse is moving.
 * \param pos The new position of the cursor on the screen.
 */
template<class Base>
bool bear::engine::item_with_input_listener<Base>::mouse_move
( const claw::math::coordinate_2d<unsigned int>& pos )
{
  const universe::position_type global
    ( this->get_level().screen_to_level( pos ) );

  if ( this->get_bounding_box().includes( global ) )
    return mouse_move_local( global - this->get_bottom_left() );
  else
    return input_listener::mouse_move( pos );
} // item_with_input_listener::mouse_move()
