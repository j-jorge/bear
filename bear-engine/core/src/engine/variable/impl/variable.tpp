/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the template methods of the
 *        bear::engine::variable class.
 * \author Julien Jorge
 */

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor
 * \param name The name of the variable.
 */
template<class T>
bear::engine::variable<T>::variable( const std::string& name )
  : base_variable(name)
{

} // variable::variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor
 * \param name The name of the variable.
 * \param val The value of the variable.
 */
template<class T>
bear::engine::variable<T>::variable( const std::string& name, const T& val )
  : base_variable(name), m_value(val)
{

} // variable::variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of the variable.
 */
template<class T>
const T& bear::engine::variable<T>::get_value() const
{
  return m_value;
} // variable::get_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assign the value to the variable in a var_map.
 * \param m The var_map in which the value is to be set.
 */
template<class T>
void bear::engine::variable<T>::assign_value_to( var_map& m ) const
{
  m.set<T>( this->get_name(), m_value );
} // variable::assign_value_to()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value to the variable from a var_map.
 * \param m The var_map in which the value is taken.
 */
template<class T>
void bear::engine::variable<T>::get_value_from( const var_map& m )
{
  CLAW_PRECOND( exists(m) );

  m_value = m.get<T>( this->get_name() );
} // variable::get_value_from()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is a value associated with the variable's name in a
 *        var_map.
 * \param m The var_map in which the value is checked.
 */
template<class T>
bool bear::engine::variable<T>::exists( const var_map& m ) const
{
  return m.exists<T>( this->get_name() );
} // variable::exists()
