/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::type_field_interval class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a copy of this instance.
 */
template<typename T>
bf::type_field* bf::type_field_interval<T>::clone() const
{
  return new type_field_interval(*this);
} // type_field_interval::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the type of the range of valid values.
 */
template<typename T>
typename bf::type_field::range_type
bf::type_field_interval<T>::get_range_type() const
{
  return field_range_interval;
} // type_field_interval::get_range_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the interval of valid values for this field.
 * \param min Minimum valid value.
 * \param max Maximum valid value.
 */
template<typename T>
void bf::type_field_interval<T>::get_interval( T& min, T& max ) const
{
  min = m_min;
  max = m_max;
} // type_field::get_interval()
