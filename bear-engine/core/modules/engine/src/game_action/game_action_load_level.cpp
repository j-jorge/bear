/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the engine::game_action_load_level class.
 * \author Julien Jorge
 */
#include "bear/engine/game_action/game_action_load_level.hpp"
#include "bear/engine/game_local_client.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param path The path of the level to load.
 */
bear::engine::game_action_load_level::game_action_load_level
( const std::string& path )
  : m_path(path)
{

} // game_action_load_level::game_action_load_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the action to a game.
 * \param the_game The game to apply to.
 */
bool bear::engine::game_action_load_level::apply( game_local_client& the_game )
{
  the_game.load_level( m_path );
  return true;
} // game_action_load_level::apply()
