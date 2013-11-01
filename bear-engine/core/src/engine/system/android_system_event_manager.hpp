/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The android_system_event_manager handles Android's application events
 *        and reacts to them by adjusting the behavior of the game.
 * \author Julien Jorge
 */
#ifdef __ANDROID__

#ifndef __ENGINE_ANDROID_SYSTEM_EVENT_MANAGER_HPP__
#define __ENGINE_ANDROID_SYSTEM_EVENT_MANAGER_HPP__

#include "engine/system/base_system_event_manager.hpp"

#include <SDL2/SDL_events.h>

namespace bear
{
  namespace engine
  {
    /**
     * \brief The android_system_event_manager handles Android's application
     *        events and reacts to them by adjusting the behavior of the game.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT android_system_event_manager:
      public base_system_event_manager
    {
    public:
      android_system_event_manager();
      ~android_system_event_manager();
      
    private:
      static int event_filter( void* user_data, SDL_Event* event );

    }; // class android_system_event_manager

  } // namespace engine
} // namespace bear

#endif // __ENGINE_ANDROID_SYSTEM_EVENT_MANAGER_HPP__

#endif // __ANDROID__

