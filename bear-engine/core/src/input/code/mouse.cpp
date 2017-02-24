/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::input::mouse class.
 * \author Julien Jorge
 */
#include "input/mouse.hpp"

#include "bear_gettext.hpp"

#include <algorithm>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
const bear::input::mouse::mouse_code bear::input::mouse::mc_left_button;
const bear::input::mouse::mouse_code bear::input::mouse::mc_middle_button;
const bear::input::mouse::mouse_code bear::input::mouse::mc_right_button;
const bear::input::mouse::mouse_code bear::input::mouse::mc_wheel_up;
const bear::input::mouse::mouse_code bear::input::mouse::mc_wheel_down;
const bear::input::mouse::mouse_code bear::input::mouse::mc_invalid;
const unsigned int bear::input::mouse::c_mouse_codes_count;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::input::mouse::mouse()
  : m_position( 0, 0 )
{

} // mouse::mouse()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a mouse_code to a human-readable string.
 * \param k The code to convert.
 */
std::string bear::input::mouse::get_name_of( mouse_code b )
{
  switch (b)
    {
    case mc_left_button: return "left click"; break;
    case mc_middle_button: return "middle click"; break;
    case mc_right_button: return "right click"; break;
    case mc_wheel_up: return "wheel up"; break;
    case mc_wheel_down: return "wheel down"; break;
    default:
      {
        CLAW_FAIL( "Invalid mouse code given to mouse::get_name_of()" );
        return "invalid mouse code";
      }
    }
} // mouse::get_name_of()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a mouse_code to a human-readable string translated with
 *        gettext.
 * \param k The code to convert.
 */
std::string bear::input::mouse::get_translated_name_of( mouse_code b )
{
  return bear_gettext( get_name_of(b).c_str() );
} // mouse::get_translated_name_of()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a mouse_code from its human-readable name.
 * \param n The name of the code.
 */
bear::input::mouse::mouse_code
bear::input::mouse::get_button_named( const std::string& n )
{
  for (mouse_code i=mc_range_min; i<=mc_range_max; ++i)
    if ( get_name_of(i) == n )
      return i;

  return mc_invalid;
} // mouse::get_button_named()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first pressed button.
 */
bear::input::mouse::const_iterator bear::input::mouse::begin() const
{
  return m_current_state.begin();
} // mouse::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator after the last pressed button.
 */
bear::input::mouse::const_iterator bear::input::mouse::end() const
{
  return m_current_state.end();
} // mouse::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if no keys are pressed.
 */
bool bear::input::mouse::empty() const
{
  return m_current_state.empty();
} // mouse::empty()

void bear::input::mouse::set_display( const display_projection& display )
{
  m_display = display;
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the mouse.
 */
const claw::math::coordinate_2d<unsigned int>&
bear::input::mouse::get_position() const
{
  return m_position;
} // mouse::get_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Re-read the status of all keys.
 * \pre The caller is an instance of bear::input::system.
 */
void bear::input::mouse::refresh()
{
  update_position();
  m_current_state.clear();

  SDL_Event e;

  // The range of events to process. It includes button up and button down.
  const SDL_EventType event_min( SDL_MOUSEMOTION );
  const SDL_EventType event_max( SDL_MOUSEWHEEL );
  
  while ( SDL_PeepEvents(&e, 1, SDL_GETEVENT, event_min, event_max ) == 1 )
    if ( e.type == SDL_MOUSEBUTTONDOWN )
      process_button_down_event( reinterpret_cast<SDL_MouseButtonEvent*>(&e) );
    else if ( e.type == SDL_MOUSEBUTTONUP )
      process_button_up_event( reinterpret_cast<SDL_MouseButtonEvent*>(&e) );
    else if ( e.type == SDL_MOUSEWHEEL )
      process_wheel_event( reinterpret_cast<SDL_MouseWheelEvent*>(&e) );

  m_current_state.insert( m_pressed_buttons.begin(), m_pressed_buttons.end() );

#ifdef __ANDROID__
  m_current_state.clear();
#endif
} // mouse::refresh()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds a button in m_pressed_buttons in response to a mouse button down
 *        event.
 * \param evt The event dispatched by the SDL.
 */
void
bear::input::mouse::process_button_down_event( const SDL_MouseButtonEvent* evt )
{
  if ( evt->state == SDL_PRESSED )
    m_pressed_buttons.insert( sdl_button_to_local( evt->button ) );
} // mouse::process_button_down_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds a button in m_pressed_buttons in response to a mouse button up
 *        event.
 * \param evt The event dispatched by the SDL.
 */
void
bear::input::mouse::process_button_up_event( const SDL_MouseButtonEvent* evt )
{
  if ( evt->state == SDL_RELEASED )
    m_pressed_buttons.erase( sdl_button_to_local( evt->button ) );
} // mouse::process_button_up_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds a button in m_pressed_buttons in response to a mouse wheel event.
 * \param evt The event dispatched by the SDL.
 */
void
bear::input::mouse::process_wheel_event( const SDL_MouseWheelEvent* evt )
{
  if ( evt->y > 0 )
    m_current_state.insert( mc_wheel_up );
  else
    m_current_state.insert( mc_wheel_down );
} // mouse::process_wheel_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the position of the mouse.
 */
void bear::input::mouse::update_position()
{
  int x, y;

  SDL_GetMouseState( &x, &y );
  m_position = m_display.window_to_display( x, y );
} // mouse::update_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a SDLK_* value to the corresponding mouse_code.
 * \param sdl_val The SDL value to convert.
 * \param shift Tell if a shift button is considered pressed.
 * \param alt Tell if an alt button is considered pressed.
 */
bear::input::mouse::mouse_code bear::input::mouse::sdl_button_to_local
( unsigned int sdl_val ) const
{
  switch(sdl_val)
    {
    case SDL_BUTTON_LEFT : return mc_left_button; break;
    case SDL_BUTTON_MIDDLE : return mc_middle_button; break;
    case SDL_BUTTON_RIGHT : return mc_right_button; break;
    default: return mc_invalid;
    }
} // mouse::sdl_mouse_to_local()
