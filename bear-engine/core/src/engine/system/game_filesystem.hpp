/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class provides an access to various game related directories
     *        and files.
 * \author Julien Jorge
 */
#ifndef __ENGINE_GAME_FILESYSTEM_HPP__
#define __ENGINE_GAME_FILESYSTEM_HPP__

#include "engine/class_export.hpp"

#include <string>

namespace bear
{
  namespace engine
  {
    class base_game_filesystem;

    /**
     * \brief This class provides an access to various game related directories
     *        and files.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT game_filesystem
    {
    public:
      game_filesystem();
      game_filesystem( const game_filesystem& that );
      game_filesystem( const base_game_filesystem& impl );
      ~game_filesystem();

      void swap( game_filesystem& that );
      game_filesystem& operator=( game_filesystem that );

      std::string get_custom_config_file_name( std::string name );
      std::string get_custom_data_file_name( std::string name );

    private:
      /** \brief The instance that effectively provides the files. */
      base_game_filesystem* m_impl;

    }; // class game_filesystem
  } // namespace engine
} // namespace bear

namespace std
{
  void swap
  ( bear::engine::game_filesystem& a, bear::engine::game_filesystem& b );
} // namespace std

#endif // __ENGINE_GAME_FILESYSTEM_HPP__
