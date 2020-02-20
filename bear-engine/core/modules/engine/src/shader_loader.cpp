/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::shader_loader class.
 * \author Julien Jorge
 */
#include "bear/engine/shader_loader.hpp"

#include "bear/engine/resource_pool.hpp"

#include <claw/exception.hpp>
#include <claw/string_algorithm.hpp>

#include <algorithm>

/*----------------------------------------------------------------------------*/
/**
 * \brief Reads the content of a shader file.
 * \param result The stream in which is written the shader program.
 * \param name The path to the resource file to read.
 */
void bear::engine::shader_loader::parse_shader_file
( std::stringstream& result, std::string name )
{
  std::vector<std::string> seen;

  load_include( result, name, seen );
} // shader_loader::parse_shader_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reads the content of a shader file.
 * \param output The stream in which is written the shader program.
 * \param file_name The path to the resource file to read.
 * \param seen The paths to the resources files that have already been included
 *        in the output.
 */
void bear::engine::shader_loader::load_include
( std::stringstream& output, std::string file_name,
  std::vector<std::string>& seen )
{
  seen.push_back( file_name );

  std::stringstream f;
  resource_pool::get_instance().get_file(file_name, f);

  std::string line;

  while ( std::getline( f, line ) )
    {
      claw::text::trim( line );

      if ( !parse_include( output, file_name, line, seen ) )
        output << line << '\n';
    }
} // shader_loader::load_include()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reads a shader file included in another shader file.
 * \param output The stream in which is written the shader program.
 * \param file_name The path to the resource file that includes the other file.
 * \param line The line to parse to get the include.
 * \param seen The paths to the resources files that have already been included
 *        in the output.
 * \return true if line is effectively an include. It such a case it will have
 *         been included in the output.
 */
bool bear::engine::shader_loader::parse_include
( std::stringstream& output, std::string file_name, std::string line,
  std::vector<std::string>& seen )
{
  std::istringstream iss( line );

  std::string pragma;
  std::string include;

  if ( (iss >> pragma) && std::getline(iss, include, '"') )
    {
      claw::text::trim(pragma);
      claw::text::trim(include);

      if ( (pragma == "#pragma") && (include == "include") )
        {
          std::string included_file;
          std::getline( iss, included_file, '"' );
          
          included_file = get_relative_file_name( file_name, included_file );

          if ( std::find( seen.begin(), seen.end(), included_file )
               == seen.end() )
            {
              load_include( output, included_file, seen );
              return true;
            }
        }
    }

  return false;
} // shader_loader::parse_include()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the path to the shader program resource included in another
 *        shader program.
 * \param reference The file that does the include.
 * \param file_name The included file.
 */
std::string bear::engine::shader_loader::get_relative_file_name
( std::string reference, std::string file_name )
{
  const std::string::size_type last_separator( reference.find_last_of( '/' ) );

  if ( last_separator == std::string::npos )
    return file_name;
  else
    return reference.substr( 0, last_separator + 1 ) + file_name;
} // shader_loader::get_relative_file_name()

