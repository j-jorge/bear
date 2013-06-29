/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::game_quit class.
 * \author Sebastien Angibaud
 */
#include "generic_items/game_quit.hpp"

#include "engine/game.hpp"
#include "engine/export.hpp"

BASE_ITEM_EXPORT( game_quit, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::game_quit::game_quit()
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // game_quit::game_quit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Activate the toggle.
 * \param activator The item that activates the toggle.
 */
void bear::game_quit::toggle_on( base_item* activator )
{
  bear::engine::game::get_instance().end();
} // game_quit::toggle_on()
