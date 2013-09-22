/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::game_initializer class.
 * \author Julien Jorge
 */
#include "engine/game_initializer.hpp"

#include <cstdlib>

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the single instance of this class.
 */
bear::engine::game_initializer& bear::engine::game_initializer::get_instance()
{
  return super::get_instance();
} // game_initializer::get_instance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the function to call when initializing the game.
 * \param f The function.
 */
bool bear::engine::game_initializer::set_init( init_game_function_type f )
{
  if ( m_init == NULL )
    {
      m_init = f;
      return true;
    }

  return false;
} // game_initializer::set_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the function to call when ending the game.
 * \param f The function.
 */
bool bear::engine::game_initializer::set_end( end_game_function_type f )
{
  if ( m_end == NULL )
    {
      m_end = f;
      return true;
    }

  return false;
} // game_initializer::set_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calls the function initializing the game, if any, and unsets the init
 *        function.
 */
void bear::engine::game_initializer::init()
{
  if ( m_init != NULL )
    {
      m_init();
      m_init = NULL;
    }
} // game_initializer::init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calls the function of the end of the game, if any, and unsets the
 *        ending function.
 */
void bear::engine::game_initializer::end()
{
  if ( m_end != NULL )
    {
      m_end();
      m_end = NULL;
    }
} // game_initializer::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs a game_initializer with the default values: no init
 *        function nor ending function.
 */
bear::engine::game_initializer::game_initializer()
  : m_init(NULL), m_end(NULL)
{

} // game_initializer::game_initializer()
