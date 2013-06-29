/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::input::joystick_button class.
 * \author Julien Jorge
 */
#include "input/joystick_button.hpp"

#include "bear_gettext.hpp"

#include <claw/string_algorithm.hpp>
#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::input::joystick_button::joystick_button()
  : joystick_index(joystick::number_of_joysticks()),
    button(joystick::jc_invalid)
{

} // joystick_button::joystick_button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param joy The index of the joystick.
 * \param b The code of the button.
 */
bear::input::joystick_button::joystick_button
( unsigned int joy, joystick::joy_code b )
  : joystick_index(joy), button(b)
{

} // joystick_button::joystick_button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two joystick_button in lexicographic order.
 * \param that joystick_button to compare to.
 */
bool
bear::input::joystick_button::operator<( const joystick_button& that ) const
{
  if ( joystick_index == that.joystick_index )
    return button < that.button;
  else
    return joystick_index < that.joystick_index;
} // joystick_button::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two joystick buttons are the same.
 * \param that joystick_button to compare to.
 */
bool
bear::input::joystick_button::operator==( const joystick_button& that ) const
{
  return ( joystick_index == that.joystick_index )
    && (button == that.button);
} // joystick_button::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a joystick_button to a human-readable string.
 * \param j The button to convert.
 */
std::string
bear::input::joystick_button::get_name_of( const joystick_button& j )
{
  std::ostringstream oss;
  oss << "joystick" << ' ' << j.joystick_index << ' '
      << joystick::get_name_of(j.button);

  return oss.str();
} // joystick_button::get_name_of()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a joystick_button to a human-readable string translated with
 *        gettext.
 * \param j The button to convert.
 */
std::string
bear::input::joystick_button::get_translated_name_of( const joystick_button& j )
{
  std::ostringstream oss;
  oss << bear_gettext("joystick") << ' ' << j.joystick_index << ' '
      << joystick::get_translated_name_of(j.button);

  return oss.str();
} // joystick_button::get_translated_name_of()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a joystick_button from its human-readable name.
 * \param n The name of the button.
 */
bear::input::joystick_button
bear::input::joystick_button::get_button_named( const std::string& n )
{
  std::istringstream iss(n);
  unsigned int j;
  std::string joy_str;

  if ( iss >> joy_str >> j )
    if ( joy_str == "joystick" )
      {
        std::string button( n.end() - iss.rdbuf()->in_avail(), n.end() );
        claw::text::trim(button);

        return joystick_button( j, joystick::get_code_named(button) );
      }

  return joystick_button(0, joystick::jc_invalid);
} // joystick_button::get_button_named()
