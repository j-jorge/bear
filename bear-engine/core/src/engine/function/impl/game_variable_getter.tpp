/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::game_variable_getter class.
 * \author Julien Jorge
 */

#include "engine/game.hpp"
#include "engine/variable/variable.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<typename T>
bear::engine::game_variable_getter<T>::game_variable_getter()
{

} // game_variable_getter::game_variable_getter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 */
template<typename T>
bear::engine::game_variable_getter<T>::game_variable_getter
(const game_variable_getter<T>& that)
  : m_name(that.m_name), m_default_value(that.m_default_value)
{
} // game_variable_getter::game_variable_getter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param var_name The name of the game variable.
 * \param default_value The default value returned if the variable is not set.
 */
template<typename T>
bear::engine::game_variable_getter<T>::game_variable_getter
( const std::string& var_name, const T& default_value )
  : m_name(var_name), m_default_value(default_value)
{

} // game_variable_getter::game_variable_getter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the variable.
 * \param n The name of the variable.
 */
template<typename T>
void bear::engine::game_variable_getter<T>::set_name( const std::string& n )
{
  m_name = n;
} // game_variable_getter::set_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the variable.
 */
template<typename T>
const std::string& bear::engine::game_variable_getter<T>::get_name() const
{
  return m_name;
} // game_variable_getter::get_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the default value, returned if the variable is not set.
 * \param v The default value.
 */
template<typename T>
void bear::engine::game_variable_getter<T>::set_default_value( const T& v )
{
  m_default_value = v;
} // game_variable_getter::set_default_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of the game variable.
 */
template<typename T>
T bear::engine::game_variable_getter<T>::operator()() const
{
  variable<T> v(m_name, m_default_value);

  if ( game::get_instance().game_variable_exists(v) )
    game::get_instance().get_game_variable(v);

  return v.get_value();
} // game_variable_getter::operator()()
