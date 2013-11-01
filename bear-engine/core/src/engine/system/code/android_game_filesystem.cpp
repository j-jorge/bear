/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::android_game_filesystem class.
 * \author Julien Jorge
 */
#ifdef __ANDROID__

#include "engine/system/android_game_filesystem.hpp"

#include <SDL2/SDL_system.h>

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param game_name The name of the game
 */
bear::engine::android_game_filesystem::android_game_filesystem
( std::string game_name )
  : default_game_filesystem( game_name, get_android_root() )
{
  claw::logger << claw::log_verbose << "Internal storage is '"
               << get_android_root() << std::endl;
} // android_game_filesystem::android_game_filesystem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a dynamically allocated copy of this instance.
 */
bear::engine::android_game_filesystem*
bear::engine::android_game_filesystem::clone() const
{
  return new android_game_filesystem( *this );
} // android_game_filesystem::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the root directory where the application can write its files.
 */
std::string bear::engine::android_game_filesystem::get_android_root() const
{
  return SDL_AndroidGetInternalStoragePath();
} // android_game_filesystem::get_android_root()

#endif // __ANDROID__
