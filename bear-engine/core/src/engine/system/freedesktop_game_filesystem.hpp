/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The freedesktop_game_filesystem class is an implementation of the
 *        base_game_filesystem class that follows the specifications of
 *        freedesktop.org.
 * \author Julien Jorge
 */
#ifndef __ENGINE_FREEDESKTOP_GAME_FILESYSTEM_HPP__
#define __ENGINE_FREEDESKTOP_GAME_FILESYSTEM_HPP__

#include "engine/system/base_game_filesystem.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The freedesktop_game_filesystem class is an implementation of the
     *        base_game_filesystem class using a single directory in the user's
     *        home directory.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT freedesktop_game_filesystem:
      public base_game_filesystem
    {
    public:
      freedesktop_game_filesystem( std::string game_name );

      virtual freedesktop_game_filesystem* clone() const;

      virtual std::string get_custom_config_file_name( std::string name );
      virtual std::string get_custom_data_file_name( std::string name );

    private:
      std::string
        get_custom_game_file( std::string name, std::string dir ) const;
      std::string get_game_directory( std::string dir ) const;

      std::string get_freedesktop_directory
        ( std::string env, std::string default_dir ) const;

    private:
      /** \brief The name of the game. */
      const std::string m_game_name;

    }; // class freedesktop_game_filesystem
  } // namespace engine
} // namespace bear

#endif // __ENGINE_FREEDESKTOP_GAME_FILESYSTEM_HPP__
