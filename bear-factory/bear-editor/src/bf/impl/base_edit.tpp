/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::base_edit class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor from a value.
 * \param v The value.
 */
template<typename Type>
bf::base_edit<Type>::base_edit( const value_type& v )
  : m_value(v)
{

} // base_edit::base_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
template<typename Type>
bf::base_edit<Type>::~base_edit()
{
  // nothing to do
} // base_edit::~base_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value.
 */
template<typename Type>
const typename bf::base_edit<Type>::value_type&
bf::base_edit<Type>::get_value() const
{
  return m_value;
} // base_edit::get_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value.
 * \param v The new value.
 */
template<typename Type>
void bf::base_edit<Type>::set_value( const value_type& v )
{
  m_value = v;
  value_updated();
} // base_edit::set_value()
