/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::text_interface::converted_argument class.
 * \author Julien Jorge.
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param v The converted value of the argument.
 */
template<typename T>
bear::text_interface::converted_argument::converted_argument( T* v )
  : m_holder(v)
{

} // converted_argument::converted_argument()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param v The converted value of the argument.
 */
template<typename T>
bear::text_interface::converted_argument::converted_argument( T& v )
  : m_holder(&v)
{

} // converted_argument::converted_argument()

/*----------------------------------------------------------------------------*/
/**
 * \brief Cast the value into a given type.
 */
template<typename T>
T bear::text_interface::converted_argument::cast_to() const
{
  return static_cast<T>( m_holder );
} // converted_argument::converted_argument()
