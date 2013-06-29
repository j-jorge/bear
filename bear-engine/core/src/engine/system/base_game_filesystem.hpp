/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The base_game_filesystem class provides an access to various game
 *        related directories and files.
 * \author Julien Jorge
 */
#ifndef __ENGINE_BASE_GAME_FILESYSTEM_HPP__
#define __ENGINE_BASE_GAME_FILESYSTEM_HPP__

#include "engine/class_export.hpp"
#include <string>

namespace bear
{
  namespace engine
  {
    /**
     * \brief The base_game_filesystem class provides an access to various game
     *        related directories and files.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT base_game_filesystem
    {
    public:
      virtual ~base_game_filesystem();

      virtual base_game_filesystem* clone() const = 0;

      virtual std::string get_custom_config_file_name( std::string name ) = 0;
      virtual std::string get_custom_data_file_name( std::string name ) = 0;

    protected:
      std::string get_name_as_filename( std::string name ) const;

    }; // class base_game_filesystem
  } // namespace engine
} // namespace bear

#endif // __ENGINE_BASE_GAME_FILESYSTEM_HPP__
