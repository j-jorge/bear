/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Tell a game to pop the current level.
 * \author Julien Jorge
 */
#ifndef __ENGINE_GAME_ACTION_POP_LEVEL_HPP__
#define __ENGINE_GAME_ACTION_POP_LEVEL_HPP__

#include "engine/game_action/game_action.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief Tell a game to pop the current level.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT game_action_pop_level:
      public game_action
    {
    public:
      bool apply( game_local_client& the_game );

    }; // class game_action_pop_level
  } // namespace engine
} // namespace bear

#endif // __ENGINE_GAME_ACTION_POP_LEVEL_HPP__
