/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The android_game_filesystem class is an implementation of the
 *        base_game_filesystem class that allow to write game files for an
 *        Android application.
 * \author Julien Jorge
 */
#ifdef __ANDROID__

#ifndef __ENGINE_ANDROID_GAME_FILESYSTEM_HPP__
#define __ENGINE_ANDROID_GAME_FILESYSTEM_HPP__

#include "engine/system/default_game_filesystem.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The android_game_filesystem class is an implementation of the
     *        base_game_filesystem class that allow to write game files for an
     *        Android application.
     * \author Julien Jorge
     */
    class android_game_filesystem:
      public default_game_filesystem
    {
    public:
      android_game_filesystem( std::string game_name );

      virtual android_game_filesystem* clone() const;

    private:
      std::string get_android_root() const;
    
    }; // class android_game_filesystem
  } // namespace engine
} // namespace bear

#endif // __ENGINE_ANDROID_GAME_FILESYSTEM_HPP__

#endif // __ANDROID__
