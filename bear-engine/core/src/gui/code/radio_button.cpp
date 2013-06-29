/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the gui::radio_button class.
 * \author Julien Jorge
 */
#include "gui/radio_button.hpp"

#include "input/keyboard.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param off The sprite displayed when the box is not checked.
 * \param on The sprite displayed when the box is checked.
 */
bear::gui::radio_button::radio_button
( const visual::sprite& off, const visual::sprite& on )
  : checkable(off, on)
{

} // radio_button::radio_button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param off The sprite displayed when the box is not checked.
 * \param on The sprite displayed when the box is checked.
 * \param f The font used to display the text.
 */
bear::gui::radio_button::radio_button
( const visual::sprite& off, const visual::sprite& on, font_type f )
  : checkable(off, on, f)
{

} // radio_button::radio_button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that a key has been pressed.
 * \param key The code of the key.
 */
bool bear::gui::radio_button::on_key_press( const bear::input::key_info& key )
{
  bool result = true;

  if ( key.is_enter() || (key.get_code() == input::keyboard::kc_space) )
    check();
  else
    result = false;

  return result;
} // radio_button::on_key_press()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that a joystick button has been pressed.
 * \param button The code of the button.
 * \param joy_index The index of the joytick.
 */
bool bear::gui::radio_button::on_button_press
( bear::input::joystick::joy_code button, unsigned int joy_index )
{
  bool result = true;

  switch( button )
    {
    case bear::input::joystick::jc_button_1:
    case bear::input::joystick::jc_button_2:
    case bear::input::joystick::jc_button_3:
    case bear::input::joystick::jc_button_4:
    case bear::input::joystick::jc_button_5:
    case bear::input::joystick::jc_button_6:
    case bear::input::joystick::jc_button_7:
    case bear::input::joystick::jc_button_8:
    case bear::input::joystick::jc_button_9:
    case bear::input::joystick::jc_button_10:
    case bear::input::joystick::jc_button_11:
    case bear::input::joystick::jc_button_12:
    case bear::input::joystick::jc_button_13:
    case bear::input::joystick::jc_button_14:
    case bear::input::joystick::jc_button_15:
    case bear::input::joystick::jc_button_16:
      check();
      break;
    default:
      result = false;
    }

  return result;
} // radio_button::on_button_press()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that a mouse button has been pressed.
 * \param key The code of the button.
 * \param pos The position of the mouse.
 */
bool bear::gui::radio_button::on_mouse_press
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  check();
  return true;
} // radio_button::on_mouse_press()
