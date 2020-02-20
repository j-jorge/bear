/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to load the shader files.
 * \author Julien Jorge
 */
#ifndef __ENGINE_SHADER_LOADER_HPP__
#define __ENGINE_SHADER_LOADER_HPP__

#include <string>
#include <vector>

#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief A class to load the shader files.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT shader_loader
    {
    public:
      static void parse_shader_file
        ( std::stringstream& result, std::string name );

    private:
      static void
        load_include
        ( std::stringstream& output, std::string name,
          std::vector<std::string>& seen );

      static bool parse_include
        ( std::stringstream& output, std::string file_name, std::string line,
          std::vector<std::string>& seen );

      static std::string get_relative_file_name
        ( std::string reference, std::string file_name );

    }; // class shader_loader

  } // namespace engine
} // namespace bear

#endif // __ENGINE_SHADER_LOADER_HPP__
