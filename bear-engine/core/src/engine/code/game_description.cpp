/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::game_description class.
 * \author Julien Jorge
 */
#include "engine/game_description.hpp"

#include "bear_gettext.hpp"

#include <string>
#include <vector>

#include <claw/assert.hpp>
#include <claw/logger.hpp>
#include <claw/string_algorithm.hpp>
#include <claw/exception.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::game_description::game_description()
  : m_game_name("Anonymous game"), m_screen_size(640, 480),
    m_active_area_margin(500), m_use_dumb_rendering(false)
{

} // game_description::game_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs the description from the command line arguments.
 * \param arg The arguments received on the command line.
 * \throws claw::exception if an argument could not be parsed. The message of
 *         the exception tells which argument has a bad value.
 */
bear::engine::game_description::game_description
( const claw::arguments_table& arg )
  : m_game_name("Anonymous game"), m_screen_size(640, 480),
    m_active_area_margin(500), m_use_dumb_rendering(false)
{
  if ( arg.has_value("--game-name") )
    set_game_name( arg.get_string("--game-name") );

  if ( arg.has_value("--active-area") )
    {
      if ( arg.only_integer_values("--active-area") )
        set_active_area_margin( arg.get_integer("--active-area") );
      else
        throw claw::exception
          ( "--active-area=" + arg.get_string("--active-area" ));
    }

  set_dumb_rendering
    ( arg.get_bool( "--dumb-rendering" )
      && !arg.get_bool( "--no-dumb-rendering" ) );

  if ( arg.has_value("--screen-height") )
    {
      if ( arg.only_integer_values("--screen-height") )
        set_screen_height( arg.get_integer("--screen-height") );
      else
        throw claw::exception
          ( "--screen-height=" + arg.get_string("--screen-height") );
    }

  if ( arg.has_value("--screen-width") )
    {
      if ( arg.only_integer_values("--screen-width") )
        set_screen_width( arg.get_integer("--screen-width") );
      else
        throw claw::exception
          ( "--screen-width=" + arg.get_string("--screen-width") );
    }
  
  if ( arg.has_value("--start-level") )
    set_start_level( arg.get_string("--start-level") );

  add_item_library( arg.get_all_of_string("--item-library") );
  add_resources_path( arg.get_all_of_string("--data-path") );
} // game_description::game_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fills a command line argument table with the argument supported by the
 *        constructors of this class.
 * \param arg The argument table in which we add the arguments.
 */
void bear::engine::game_description::get_valid_command_line_arguments
( claw::arguments_table& arg )
{
  arg.add_long
    ( "--game-name", bear_gettext("The name of the game."), true,
      bear_gettext("string") );
  arg.add_long
    ( "--active-area",
      bear_gettext
      ("The margin around the camera in which we check for activity."), true,
      bear_gettext("integer") );
  arg.add_long
    ( "--screen-width", bear_gettext("The width of the screen."), true,
      bear_gettext("integer") );
  arg.add_long
    ( "--screen-height", bear_gettext("The height of the screen."), true,
      bear_gettext("integer") );
  arg.add_long
    ( "--data-path",
      bear_gettext("Path to the directory containing the data of the game."),
      true, bear_gettext("path") );
  arg.add_long
    ( "--dumb-rendering",
      bear_gettext("Tells to use the dumbest rendering procedure."), true );
  arg.add_long
    ( "--no-dumb-rendering",
      bear_gettext("Tells not to use the dumbest rendering procedure."), true );
  arg.add_long
    ( "--item-library",
      bear_gettext("Path to a library containing items for the game."), true,
      bear_gettext("path") );
  arg.add_long
    ( "--start-level", bear_gettext("The path of the first level to run."),
      true, bear_gettext("string") );
} // game_description::get_valid_command_line_arguments()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the first level to load.
 */
const std::string& bear::engine::game_description::start_level() const
{
  return m_start_level;
} // game_description::start_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the game.
 */
const std::string& bear::engine::game_description::game_name() const
{
  return m_game_name;
} // game_description::game_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the screen.
 */
const claw::math::coordinate_2d<unsigned int>&
bear::engine::game_description::screen_size() const
{
  return m_screen_size;
} // game_description::screen_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if we use dumb rendering by default.
 */
bool bear::engine::game_description::dumb_rendering() const
{
  return m_use_dumb_rendering;
} // game_description::dumb_rendering()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the margin of the active area around the screen.
 */
double  bear::engine::game_description::active_area_margin() const
{
  return m_active_area_margin;
} // game_description::active_area_margin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the paths to the forder containing the resources.
 */
const bear::engine::game_description::string_list&
bear::engine::game_description::resources_path() const
{
  return m_resources_path;
} // game_description::resources_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the paths to the libraries to link to.
 */
const bear::engine::game_description::string_list&
bear::engine::game_description::libraries() const
{
  return m_libraries;
} // game_description::libraries()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the game.
 * \param value The name of the game.
 */
void bear::engine::game_description::set_game_name( const std::string& value )
{
  m_game_name = value;
} // game_description::set_game_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the width of the screen.
 * \param value The width of the screen.
 */
void bear::engine::game_description::set_screen_width( unsigned int value )
{
  m_screen_size.x = value;
} // game_description::set_screen_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the height of the screen.
 * \param value The height of the screen.
 */
void bear::engine::game_description::set_screen_height( unsigned int value )
{
  m_screen_size.y = value;
} // game_description::set_screen_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells to use the dumb rendering procedure.
 * \param v Tells to use it or not.
 */
void bear::engine::game_description::set_dumb_rendering( bool v )
{
  m_use_dumb_rendering = v;
} // game_description::set_dumb_rendering()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the margin of the active_area around the screen.
 * \param value The size of the margin.
 */
void bear::engine::game_description::set_active_area_margin
( unsigned int value )
{
  m_active_area_margin = value;
} // game_description::set_active_area_margin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds a path to a directory where the game's resources can be found.
 * \param value The path to the resources' directory.
 */
void bear::engine::game_description::add_resources_path
( const std::string& value )
{
  m_resources_path.push_back( value );
} // game_description::add_resources_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the paths to the directory where the game's resources can be
 *        found.
 * \param value The paths to the resources' directories.
 */
void bear::engine::game_description::add_resources_path
( const string_list& value )
{
  m_resources_path.insert( m_resources_path.end(), value.begin(), value.end() );
} // game_description::add_resources_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds a library that must be loaded to launch the game.
 * \param value The name of the library.
 */
void bear::engine::game_description::add_item_library
( const std::string& value )
{
  m_libraries.push_back( value );
} // game_description::add_item_library()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the libraries that must be loaded to launch the game.
 * \param value The names of the libraries.
 */
void bear::engine::game_description::add_item_library
( const string_list& value )
{
  m_libraries.insert( m_libraries.end(), value.begin(), value.end() );
} // game_description::add_item_library()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the first level.
 * \param value The name of the first level.
 */
void bear::engine::game_description::set_start_level( const std::string& value )
{
  m_start_level = value;
} // game_description::set_start_level()
