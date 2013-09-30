/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::android_system_event_manager
 *        class.
 * \author Julien Jorge
 */
#ifdef __ANDROID__

#include "engine/system/android_system_event_manager.hpp"

#include "engine/game.hpp"

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs the instance. Setup the event handling with the SDL.
 */
bear::engine::android_system_event_manager::android_system_event_manager()
{
  SDL_EventState( SDL_APP_TERMINATING, SDL_ENABLE );
  SDL_EventState( SDL_APP_LOWMEMORY, SDL_ENABLE );
  SDL_EventState( SDL_APP_WILLENTERBACKGROUND, SDL_ENABLE );
  SDL_EventState( SDL_APP_DIDENTERBACKGROUND, SDL_ENABLE );
  SDL_EventState( SDL_APP_WILLENTERFOREGROUND, SDL_ENABLE );
  SDL_EventState( SDL_APP_DIDENTERFOREGROUND, SDL_ENABLE );

  SDL_SetEventFilter( &event_filter, this );
} // android_system_event_manager::android_system_event_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destroys the instance. Cleans up the event handling with the SDL.
 */
bear::engine::android_system_event_manager::~android_system_event_manager()
{
  SDL_EventFilter current_filter;
  void* user_data;

  if ( SDL_GetEventFilter( &current_filter, &user_data ) )
    if ( ( current_filter == &event_filter )
         && ( user_data == this ) )
      SDL_SetEventFilter( NULL, NULL );
} // android_system_event_manager::~android_system_event_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Processes an event prepared by the sdl. We handle the application
 *        events and let the SDL to process the others.
 * \param g The running instance of rp::game.
 * \param event The event to check.
 */
int bear::engine::android_system_event_manager::event_filter
( void* user_data, SDL_Event* event )
{
  int result(0);

  switch ( event->type )
    {
    case SDL_APP_TERMINATING:
      claw::logger << claw::log_verbose
                   << "Ignoring application event: SDL_APP_TERMINATING"
                   << std::endl;
      break;

    case SDL_APP_LOWMEMORY:
      claw::logger << claw::log_verbose
                   << "Ignoring application event: SDL_APP_LOWMEMORY"
                   << std::endl;
      break;

    case SDL_APP_WILLENTERBACKGROUND:
      game::get_instance().sleep();
      break;

    case SDL_APP_DIDENTERBACKGROUND:
      claw::logger << claw::log_verbose
                   << "Ignoring application event: SDL_APP_DIDENTERBACKGROUND"
                   << std::endl;
      break;

    case SDL_APP_WILLENTERFOREGROUND:
      game::get_instance().wake_up();
      break;

    case SDL_APP_DIDENTERFOREGROUND:
      claw::logger << claw::log_verbose
                   << "Ignoring application event: SDL_APP_DIDENTERFOREGROUND"
                   << std::endl;
      break;

    default:
      result = 1;
    }

  return result;
} // android_system_event_manager::event_filter()

#endif // __ANDROID__

