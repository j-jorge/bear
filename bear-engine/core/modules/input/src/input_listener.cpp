/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::input::input_listener class.
 * \author Julien Jorge
 */
#include "bear/input/input_listener.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::input::input_listener::~input_listener()
{
  // nothing to do
} // input_listener::~input_listener()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when a key is pressed.
 * \param key The value of the key.
 * \return true if the key has been processed.
 *
 * This methods use raw key codes as \a key. If you want the character as
 * entered by the user, use char_pressed.
 */
bool bear::input::input_listener::key_pressed( const key_info& key )
{
  return false;
} // input_listener::key_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when a key is released.
 * \param key The value of the key.
 * \return true if the key has been processed.
 *
 * This methods use raw key codes as \a key. If you want the character as
 * entered by the user, use char_pressed.
 */
bool bear::input::input_listener::key_released( const key_info& key )
{
  return false;
} // input_listener::key_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when a key is maintained.
 * \param key The value of the key.
 * \return true if the key has been processed.
 *
 * This methods use raw key codes as \a key. If you want the character as
 * entered by the user, use char_pressed.
 */
bool bear::input::input_listener::key_maintained( const key_info& key )
{
  return false;
} // input_listener::key_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when a character is entered by the user.
 * \param key The value of the key.
 * \return true if the key has been processed.
 */
bool bear::input::input_listener::char_pressed( const key_info& key )
{
  return false;
} // input_listener::char_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when a joystick button is pressed.
 * \param button The value of the button.
 * \param joy_index The index of the joystick.
 * \return true if the button has been processed.
 */
bool bear::input::input_listener::button_pressed
( joystick::joy_code button, unsigned int joy_index )
{
  return false;
} // input_listener::button_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when a joystick button is released.
 * \param button The value of the button.
 * \param joy_index The index of the joystick.
 * \return true if the button has been processed.
 */
bool bear::input::input_listener::button_released
( joystick::joy_code button, unsigned int joy_index )
{
  return false;
} // input_listener::button_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when a joystick button is maintained.
 * \param button The value of the button.
 * \param joy_index The index of the joystick.
 * \return true if the button has been processed.
 */
bool bear::input::input_listener::button_maintained
( joystick::joy_code button, unsigned int joy_index )
{
  return false;
} // input_listener::button_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when a mouse button is pressed.
 * \param button The code of the button.
 * \param pos The current position of the cursor.
 * \return true if the button has been processed.
 */
bool bear::input::input_listener::mouse_pressed
( mouse::mouse_code button, const claw::math::coordinate_2d<unsigned int>& pos )
{
  return false;
} // input_listener::mouse_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when a mouse button is released.
 * \param button The code of the button.
 * \param pos The current position of the cursor.
 * \return true if the button has been processed.
 */
bool bear::input::input_listener::mouse_released
( mouse::mouse_code button, const claw::math::coordinate_2d<unsigned int>& pos )
{
  return false;
} // input_listener::mouse_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when a mouse button is maintained.
 * \param button The code of the button.
 * \param pos The current position of the cursor.
 * \return true if the button has been processed.
 */
bool bear::input::input_listener::mouse_maintained
( mouse::mouse_code button, const claw::math::coordinate_2d<unsigned int>& pos )
{
  return false;
} // input_listener::mouse_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when the position of the mouse changes.
 * \param pos The new position of the cursor.
 * \return true if the event has been processed.
 */
bool bear::input::input_listener::mouse_move
( const claw::math::coordinate_2d<unsigned int>& pos )
{
  return false;
} // input_listener::mouse_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called a finger related event occurs.
 * \param event The event.
 * \return true if the event has been processed.
 */
bool bear::input::input_listener::finger_action( const finger_event& event )
{
  return false;
} // input_listener::finger_action()
