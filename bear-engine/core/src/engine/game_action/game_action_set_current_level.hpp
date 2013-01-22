/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Change the current level.
 * \author Julien Jorge
 */
#ifndef __ENGINE_GAME_ACTION_SET_CURRENT_LEVEL_HPP__
#define __ENGINE_GAME_ACTION_SET_CURRENT_LEVEL_HPP__

#include "engine/game_action/game_action.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class level;

    /**
     * \brief Change the current level.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT game_action_set_current_level:
      public game_action
    {
    public:
      game_action_set_current_level( level* the_level );

      bool apply( game_local_client& the_game );

    private:
      /** \brief The level to set.. */
      level* m_level;

    }; // class game_action_set_current_level
  } // namespace engine
} // namespace bear

#endif // __ENGINE_GAME_ACTION_SET_CURRENT_LEVEL_HPP__
