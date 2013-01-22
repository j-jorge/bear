/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::system_music_toggle class.
 * \author Julien Jorge
 */
#include "generic_items/system/system_music_toggle.hpp"

#include "engine/game.hpp"

BASE_ITEM_EXPORT( system_music_toggle, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the state of the item.
 * \param elapsed_time The elapsed time since the last call.
 */
void bear::system_music_toggle::progress( universe::time_type elapsed_time )
{
  super::progress( elapsed_time );

  progress_input_reader( elapsed_time );

  toggle( !bear::engine::game::get_instance().get_music_muted(), this );
} // system_music_toggle::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief The mouse has been pressed on the item.
 * \param button The pressed button.
 * \param pos The position of the mouse on the item.
 */
bool bear::system_music_toggle::mouse_pressed_local
( input::mouse::mouse_code button, const universe::position_type& pos )
{
  super::mouse_pressed_local( button, pos );

  bear::engine::game::get_instance().set_music_muted
    ( !bear::engine::game::get_instance().get_music_muted() );

  return true;
} // system_music_toggle::mouse_pressed_local()
