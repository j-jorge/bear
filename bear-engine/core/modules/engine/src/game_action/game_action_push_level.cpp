/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the engine::game_action_push_level class.
 * \author Julien Jorge
 */
#include "bear/engine/game_action/game_action_push_level.hpp"
#include "bear/engine/game_local_client.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param path The path of the level to push.
 */
bear::engine::game_action_push_level::game_action_push_level
( const std::string& path )
  : m_path(path)
{

} // game_action_push_level::game_action_push_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the action to a game.
 * \param the_game The game to apply to.
 */
bool bear::engine::game_action_push_level::apply( game_local_client& the_game )
{
  the_game.do_push_level( m_path );
  return true;
} // game_action_push_level::apply()
