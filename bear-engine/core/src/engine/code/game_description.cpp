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

#include <string>
#include <vector>

#include <claw/assert.hpp>
#include <claw/logger.hpp>
#include <claw/string_algorithm.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::game_description::game_description()
  : m_game_name("Anonymous game"), m_screen_size(640, 480),
    m_active_area_margin(500), m_use_dumb_rendering(false)
{

} // game_description()

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
 * \brief Set the name of the first level.
 * \param value The name of the first level.
 */
void bear::engine::game_description::set_start_level( const std::string& value )
{
  m_start_level = value;
} // game_description::set_start_level()
