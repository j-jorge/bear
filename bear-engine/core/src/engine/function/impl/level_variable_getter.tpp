/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::level_variable_getter class.
 * \author Julien Jorge
 */

#include "engine/level.hpp"
#include "engine/variable/variable.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<typename T>
bear::engine::level_variable_getter<T>::level_variable_getter()
  : m_level(NULL)
{

} // level_variable_getter::level_variable_getter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 */
template<typename T>
bear::engine::level_variable_getter<T>::level_variable_getter
(const level_variable_getter<T>& that)
  : m_level(that.m_level), m_name(that.m_name),
    m_default_value(that.m_default_value)
{

} // level_variable_getter::level_variable_getter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param lvl The level where we search for the variable.
 * \param var_name The name of the level variable.
 * \param default_value The default value returned if the variable is not set.
 */
template<typename T>
bear::engine::level_variable_getter<T>::level_variable_getter
( const level* lvl, const std::string& var_name, const T& default_value )
  : m_level(lvl), m_name(var_name), m_default_value(default_value)
{

} // level_variable_getter::level_variable_getter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the level where we search for the variable.
 * \param lvl The level where we search for the variable.
 */
template<typename T>
void bear::engine::level_variable_getter<T>::set_level( const level* lvl )
{
  m_level = lvl;
} // level_variable_getter::set_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the variable.
 * \param n The name of the variable.
 */
template<typename T>
void bear::engine::level_variable_getter<T>::set_name( const std::string& n )
{
  m_name = n;
} // level_variable_getter::set_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the variable.
 */
template<typename T>
const std::string& bear::engine::level_variable_getter<T>::get_name() const
{
  return m_name;
} // level_variable_getter::get_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the default value, returned if the variable is not set.
 * \param v The default value.
 */
template<typename T>
void bear::engine::level_variable_getter<T>::set_default_value( const T& v )
{
  m_default_value = v;
} // level_variable_getter::set_default_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of the level variable.
 */
template<typename T>
T bear::engine::level_variable_getter<T>::operator()() const
{
  if ( m_level == NULL )
    return m_default_value;
  else
    {
      variable<T> v(m_name, m_default_value);

      if ( m_level->level_variable_exists(v) )
        m_level->get_level_variable(v);

      return v.get_value();
    }
} // level_variable_getter::operator()()
