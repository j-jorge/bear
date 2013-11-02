/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The default_system_event_manager handles default's application events
 *        and reacts to them by adjusting the behavior of the game.
 * \author Julien Jorge
 */
#ifndef __ENGINE_DEFAULT_SYSTEM_EVENT_MANAGER_HPP__
#define __ENGINE_DEFAULT_SYSTEM_EVENT_MANAGER_HPP__

#include "engine/system/base_system_event_manager.hpp"

#include <SDL2/SDL_events.h>

namespace bear
{
  namespace engine
  {
    /**
     * \brief The default_system_event_manager handles default's application
     *        events and reacts to them by adjusting the behavior of the game.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT default_system_event_manager:
      public base_system_event_manager
    {
    public:
      default_system_event_manager();
      ~default_system_event_manager();
      
      void refresh();

    private:
      static int event_filter( void* user_data, SDL_Event* event );
      static int process_window_event( const SDL_WindowEvent& event );

    }; // class default_system_event_manager

  } // namespace engine
} // namespace bear

#endif // __ENGINE_DEFAULT_SYSTEM_EVENT_MANAGER_HPP__
