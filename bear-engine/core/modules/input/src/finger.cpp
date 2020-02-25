/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::input::finger class.
 * \author Julien Jorge
 */
#include "bear/input/finger.hpp"

#include <SDL2/SDL.h>

void bear::input::finger::set_display( const display_projection& display )
{
  m_display = display;
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the last events.
 */
const bear::input::finger::event_list& bear::input::finger::get_events() const
{
  return m_events;
} // finger::get_events()

/*----------------------------------------------------------------------------*/
/**
 * \brief Re-read the status of all keys.
 * \pre The caller is an instance of bear::input::system.
 */
void bear::input::finger::refresh()
{
  m_events.clear();

  SDL_Event e;

  // The range of events to process. It includes button up and button down.
  const SDL_EventType event_min( SDL_FINGERDOWN );
  const SDL_EventType event_max( SDL_FINGERMOTION );
  
  while ( SDL_PeepEvents(&e, 1, SDL_GETEVENT, event_min, event_max ) == 1 )
    {
      const SDL_TouchFingerEvent* const evt =
          reinterpret_cast<SDL_TouchFingerEvent*>(&e);
      const position_type position( convert_position( evt->x, evt->y ) );

      if ( e.type == SDL_FINGERDOWN )
        m_events.push_back
          ( finger_event::create_pressed_event( position, evt->fingerId ) );
      else if ( e.type == SDL_FINGERUP )
        m_events.push_back
          ( finger_event::create_released_event( position, evt->fingerId ) );
      else if ( e.type == SDL_FINGERMOTION )
        m_events.push_back
          ( finger_event::create_motion_event
            ( position, evt->fingerId, convert_delta( evt->dx, evt->dy ) ) );
    }
} // finger::refresh()

/*----------------------------------------------------------------------------*/
/**
 * \brief Converts SDL's finger position into the coordinates of the engine.
 */
bear::input::position_type
bear::input::finger::convert_position( double x, double y ) const
{
  return m_display.window_ratio_to_display( x, y );
} // finger::convert_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Converts SDL's finger movement into the coordinates of the engine.
 */
bear::input::position_type
bear::input::finger::convert_delta( double x, double y ) const
{
  return m_display.window_offset_to_display( x, y );
} // finger::convert_delta()
