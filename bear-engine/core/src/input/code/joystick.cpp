/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::input::joystick class.
 * \author Julien Jorge
 */
#include "input/joystick.hpp"

#include "bear_gettext.hpp"

#include <SDL/SDL.h>
#include <claw/assert.hpp>
#include <claw/exception.hpp>
#include <claw/logger.hpp>
#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of plugged joysticks.
 */
unsigned int bear::input::joystick::number_of_joysticks()
{
  return SDL_NumJoysticks();
} // joystick::number_of_joysticks()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::input::joystick::joystick( unsigned int joy_id )
  : m_id(joy_id)
{
  CLAW_PRECOND( joy_id < number_of_joysticks() );

  const char* name( SDL_JoystickNameForIndex(joy_id) );

  if ( name != NULL )
    claw::logger << claw::log_verbose << "Creating joystick number " << joy_id
                 << ": \"" << name << "\"." << std::endl;

  m_joystick = SDL_JoystickOpen(joy_id);

  if (!m_joystick)
    throw claw::exception( SDL_GetError() );
} // joystick::joystick()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::input::joystick::~joystick()
{
  SDL_JoystickClose(m_joystick);
} // joystick::~joystick()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a joy_code to a human-readable string.
 * \param k The button to convert.
 */
std::string bear::input::joystick::get_name_of( joy_code k )
{
  std::string result;

  switch(k)
    {
    case jc_axis_up:         result = "up"; break;
    case jc_axis_down:       result = "down"; break;
    case jc_axis_left:       result = "left"; break;
    case jc_axis_right:      result = "right"; break;
    case jc_axis_up_left:    result = "up left"; break;
    case jc_axis_up_right:   result = "up right"; break;
    case jc_axis_down_left:  result = "down left"; break;
    case jc_axis_down_right: result = "down right"; break;
    default:
      {
        std::ostringstream oss;
        oss << "button" << ' ' << (k - jc_button_1 + 1);
        result = oss.str();
      }
    }
  return result;
} // joystick::get_name_of()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a joy_code to a human-readable string translated with gettext.
 * \param k The button to convert.
 */
std::string bear::input::joystick::get_translated_name_of( joy_code k )
{
  std::string result;

  if ( k >= jc_button_1 )
    {
      std::ostringstream oss;
      oss << bear_gettext("button") << ' ' << (k - jc_button_1 + 1);
      result = oss.str();
    }
  else
    result = bear_gettext( get_name_of(k).c_str() );

  return result;
} // joystick::get_translated_name_of()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a joy_code from its human-readable name.
 * \param n The name of the code.
 */
bear::input::joystick::joy_code
bear::input::joystick::get_code_named( const std::string& n )
{
  for (joy_code i=jc_range_min; i<=jc_range_max; ++i)
    if ( get_name_of(i) == n )
      return i;

  return jc_invalid;
} // joystick::get_code_named()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first pressed button.
 */
bear::input::joystick::const_iterator bear::input::joystick::begin() const
{
  return m_pressed_buttons.begin();
} // joystick::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator after the last pressed button.
 */
bear::input::joystick::const_iterator bear::input::joystick::end() const
{
  return m_pressed_buttons.end();
} // joystick::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if no buttons are pressed.
 */
bool bear::input::joystick::empty() const
{
  return m_pressed_buttons.empty();
} // joystick::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the identifier of the joystick.
 */
unsigned int bear::input::joystick::get_id() const
{
  return m_id;
} // joystick::get_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Re-read the status of all buttons.
 * \pre The caller is an instance of bear::input::system.
 */
void bear::input::joystick::refresh()
{
  m_pressed_buttons.clear();
  SDL_JoystickUpdate();

  joy_code axis = get_pressed_axis();

  if (axis != jc_invalid)
    m_pressed_buttons.push_back( axis );

  unsigned int num_buttons = c_number_of_buttons;

  if ( num_buttons > (unsigned int)SDL_JoystickNumButtons(m_joystick) )
    num_buttons = SDL_JoystickNumButtons(m_joystick);

  for (unsigned int button=0; button!=num_buttons; ++button)
    if ( SDL_JoystickGetButton(m_joystick, button) )
      if ( sdl_button_to_local(button) != jc_invalid )
        m_pressed_buttons.push_back( sdl_button_to_local(button) );
} // joystick::refresh()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the currently pressed axis.
 */
bear::input::joystick::joy_code bear::input::joystick::get_pressed_axis() const
{
  const bool up    = SDL_JoystickGetAxis( m_joystick, 1 ) < -15000;
  const bool down  = SDL_JoystickGetAxis( m_joystick, 1 ) > 15000;
  const bool left  = SDL_JoystickGetAxis( m_joystick, 0 ) < -15000;
  const bool right = SDL_JoystickGetAxis( m_joystick, 0 ) > 15000;

  joy_code result = jc_invalid;

  if (up)
    {
      if (left)
        result = jc_axis_up_left;
      else if (right)
        result = jc_axis_up_right;
      else
        result = jc_axis_up;
    }
  else if (down)
    {
      if (left)
        result = jc_axis_down_left;
      else if (right)
        result = jc_axis_down_right;
      else
        result = jc_axis_down;
    }
  else if (left)
    result = jc_axis_left;
  else if (right)
    result = jc_axis_right;

  return result;
} // joystick::get_pressed_axis()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a SDL_Joystick button index value to the corresponding
 *        joy_code.
 */
bear::input::joystick::joy_code
bear::input::joystick::sdl_button_to_local( unsigned int sdl_val ) const
{
  unsigned int b = jc_button_1 + sdl_val;

  if (b > c_number_of_buttons)
    return jc_invalid;
  else
    return b;
} // joystick::sdl_button_to_local()
