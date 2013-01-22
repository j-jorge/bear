/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::default_game_filesystem class.
 * \author Julien Jorge
 */
#include "engine/system/default_game_filesystem.hpp"

#include <claw/logger.hpp>
#include <claw/system_info.hpp>
#include <boost/filesystem/convenience.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param game_name The name of the game
 */
bear::engine::default_game_filesystem::default_game_filesystem
( std::string game_name )
  : m_game_name( game_name )
{

} // default_game_filesystem::default_game_filesystem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a dynamically allocated copy of this instance.
 */
bear::engine::default_game_filesystem*
bear::engine::default_game_filesystem::clone() const
{
  return new default_game_filesystem( *this );
} // default_game_filesystem::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the full path of a configuration file of the game.
 * \param name The name of the configuration file.
 */
std::string
bear::engine::default_game_filesystem::get_custom_config_file_name
( std::string name )
{
  return get_custom_game_file( name );
} // default_game_filesystem::get_custom_config_file_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the full path of a file generated by the user in the game.
 * \param name The name of the file.
 */
std::string
bear::engine::default_game_filesystem::get_custom_data_file_name
( std::string name )
{
  return get_custom_game_file( name );
} // default_game_filesystem::get_custom_data_file_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the path of a file in the user's game directory.
 * \param name The name of the file.
 */
std::string bear::engine::default_game_filesystem::get_custom_game_file
( std::string name ) const
{
  std::string result = get_game_directory();

  if ( !result.empty() )
    {
      boost::filesystem::path path( result, boost::filesystem::native );
      path /= name;
      result = path.string();
    }
  else
    result = name;

  return result;
} // default_game_filesystem::get_custom_game_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets, and creates, the personal game directory of the user.
 */
std::string bear::engine::default_game_filesystem::get_game_directory() const
{
  boost::filesystem::path dir
    ( claw::system_info::get_user_directory(), boost::filesystem::native );

  std::string result;
  std::string subdir = '.' + get_name_as_filename( m_game_name );

  dir /= boost::filesystem::path(subdir, boost::filesystem::native);

  try
    {
      boost::filesystem::create_directories(dir);
      result = dir.string();
    }
  catch( std::exception& e )
    {
      claw::logger << claw::log_error << "Can't create game directory '"
                   << dir.string() << "': " << e.what() << std::endl;
    }

  return result;
} // default_game_filesystem::get_game_directory()
