/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the engine::game_action_set_current_level class.
 * \author Julien Jorge
 */
#include "engine/game_action/game_action_set_current_level.hpp"
#include "engine/game_local_client.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param the_level The level to set.
 */
bear::engine::game_action_set_current_level::game_action_set_current_level
( level* the_level )
  : m_level(the_level)
{
  CLAW_PRECOND( the_level != NULL );
} // game_action_set_current_level::game_action_set_current_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the action to a game.
 * \param the_game The game to apply to.
 */
bool bear::engine::game_action_set_current_level::apply
( game_local_client& the_game )
{
  the_game.set_current_level( m_level );
  return true;
} // game_action_set_current_level::apply()
