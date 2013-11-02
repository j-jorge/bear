/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The base_system_event_manager class is an interface of the classes
 *        handling the system events in order to adjust the behavior of the
 *        game.
 * \author Julien Jorge
 */
#ifndef __ENGINE_BASE_SYSTEM_EVENT_MANAGER_HPP__
#define __ENGINE_BASE_SYSTEM_EVENT_MANAGER_HPP__

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The base_system_event_manager class is an interface of the classes
     *        handling the system events in order to adjust the behavior of the
     *        game.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT base_system_event_manager
    {
    public:
      virtual ~base_system_event_manager() {};

      virtual void refresh() = 0;

    }; // class base_system_event_manager
  } // namespace engine
} // namespace bear

#endif // __ENGINE_BASE_SYSTEM_EVENT_MANAGER_HPP__
