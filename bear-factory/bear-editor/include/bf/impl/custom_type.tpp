/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::custom_type class.
 * \author Julien Jorge
 */
#include "bf/compiled_file.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor with initialisation
 */
template<typename Type>
bf::custom_type<Type>::custom_type( const value_type& value )
  : m_value(value)
{

} // custom_type::custom_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of the variable.
 * \param value The new value.
 */
template<typename Type>
void bf::custom_type<Type>::set_value( const value_type& value )
{
  m_value = value;
} // custom_type::set_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value.
 */
template<typename Type>
const typename bf::custom_type<Type>::value_type&
bf::custom_type<Type>::get_value() const
{
  return m_value;
} // custom_type::get_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output the value in the compiled file.
 * \param cf The file to write in.
 */
template<typename Type>
void bf::custom_type<Type>::compile( compiled_file& cf ) const
{
  cf << m_value;
} // custom_type::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two custom_type objects are equal.
 * \param that The item to compare to.
 */
template<typename Type>
bool bf::custom_type<Type>::operator==( const custom_type<Type>& that ) const
{
  return get_value() == that.get_value();
} // custom_type::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two custom_type objects are different.
 * \param that The item to compare to.
 */
template<typename Type>
bool bf::custom_type<Type>::operator!=( const custom_type<Type>& that ) const
{
  return get_value() != that.get_value();
} // custom_type::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stream output operator.
 * \param os The stream in which we write.
 * \param v The value to write in the stream.
 */
template<typename Type>
std::ostream& operator<<( std::ostream& os, const bf::custom_type<Type>& v )
{
  return os << v.get_value();
} // operator<<()
