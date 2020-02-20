/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Base class for actions that can be applied to a game.
 * \author Julien Jorge
 */
#ifndef __ENGINE_GAME_ACTION_HPP__
#define __ENGINE_GAME_ACTION_HPP__

#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    // forward declaration
    class game_local_client;

    /**
     * \brief Base class for actions that can be applied to a game.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT game_action
    {
    public:
      virtual ~game_action() {}

      virtual bool apply( game_local_client& the_game ) = 0;

    }; // class game_action
  } // namespace engine
} // namespace bear

#endif // __ENGINE_GAME_ACTION_HPP__
