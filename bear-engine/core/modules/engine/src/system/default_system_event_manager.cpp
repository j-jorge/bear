/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::default_system_event_manager
 *        class.
 * \author Julien Jorge
 */
#include "bear/engine/system/default_system_event_manager.hpp"

#include "bear/engine/game.hpp"

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs the instance. Setup the event handling with the SDL.
 */
bear::engine::default_system_event_manager::default_system_event_manager()
{
  SDL_EventState( SDL_WINDOWEVENT, SDL_ENABLE );
  SDL_EventState( SDL_QUIT, SDL_ENABLE );

  SDL_SetEventFilter( &event_filter, this );
} // default_system_event_manager::default_system_event_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destroys the instance. Cleans up the event handling with the SDL.
 */
bear::engine::default_system_event_manager::~default_system_event_manager()
{
  SDL_EventFilter current_filter;
  void* user_data;

  if ( SDL_GetEventFilter( &current_filter, &user_data ) )
    if ( ( current_filter == &event_filter )
         && ( user_data == this ) )
      SDL_SetEventFilter( NULL, NULL );
} // default_system_event_manager::~default_system_event_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Processes the pending events.
 */
void bear::engine::default_system_event_manager::refresh()
{
  SDL_PumpEvents();
} // default_system_event_manager::refresh()

/*----------------------------------------------------------------------------*/
/**
 * \brief Processes an event prepared by the sdl. We handle the application
 *        events and let the SDL to process the others.
 * \param g The running instance of rp::game.
 * \param event The event to check.
 */
int bear::engine::default_system_event_manager::event_filter
( void* user_data, SDL_Event* event )
{
  int result(0);

  switch ( event->type )
    {
    case SDL_QUIT:
      game::get_instance().end();
      break;

    case SDL_WINDOWEVENT:
      result = process_window_event( event->window );
      break;

    default:
      result = 1;
    }

  return result;
} // default_system_event_manager::event_filter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Process an event occuring on the main window.
 * \param event The event to process.
 */
int bear::engine::default_system_event_manager::process_window_event
( const SDL_WindowEvent& event )
{
  switch( event.event )
    {
    case SDL_WINDOWEVENT_MINIMIZED:
      game::get_instance().sleep();
      break;

    case SDL_WINDOWEVENT_RESTORED:
      game::get_instance().wake_up();
      break;
    }

  return 0;
} // default_system_event_manager::process_window_event()
