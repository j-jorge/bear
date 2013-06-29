/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Tell a game to push a level.
 * \author Julien Jorge
 */
#ifndef __ENGINE_GAME_ACTION_PUSH_LEVEL_HPP__
#define __ENGINE_GAME_ACTION_PUSH_LEVEL_HPP__

#include "engine/game_action/game_action.hpp"

#include <string>

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief Tell a game to push a level.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT game_action_push_level:
      public game_action
    {
    public:
      game_action_push_level( const std::string& path );

      bool apply( game_local_client& the_game );

    private:
      /** \brief The path of the level to push. */
      std::string m_path;

    }; // class game_action_push_level
  } // namespace engine
} // namespace bear

#endif // __ENGINE_GAME_ACTION_PUSH_LEVEL_HPP__
