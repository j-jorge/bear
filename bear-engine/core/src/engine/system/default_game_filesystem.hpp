/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The default_game_filesystem class is an implementation of the
 *        base_game_filesystem class using a single directory in the user's home
 *        directory.
 * \author Julien Jorge
 */
#ifndef __ENGINE_DEFAULT_GAME_FILESYSTEM_HPP__
#define __ENGINE_DEFAULT_GAME_FILESYSTEM_HPP__

#include "engine/system/base_game_filesystem.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The default_game_filesystem class is an implementation of the
     *        base_game_filesystem class using a single directory in the user's
     *        home directory.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT default_game_filesystem:
      public base_game_filesystem
    {
    public:
      explicit default_game_filesystem( std::string game_name );
      default_game_filesystem( std::string game_name, std::string root );

      virtual default_game_filesystem* clone() const;

      virtual std::string get_custom_config_file_name( std::string name );
      virtual std::string get_custom_data_file_name( std::string name );

    private:
      std::string get_custom_game_file( std::string name ) const;
      std::string get_game_directory() const;

    private:
      /** \brief The name of the game. */
      const std::string m_game_name;

      /** brief The root directory where the files are stored. */
      std::string m_root;

    }; // class default_game_filesystem
  } // namespace engine
} // namespace bear

#endif // __ENGINE_DEFAULT_GAME_FILESYSTEM_HPP__
