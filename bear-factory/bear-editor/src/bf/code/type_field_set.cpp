/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::type_field_set class.
 * \author Julien Jorge
 */
#include "bf/type_field_set.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the field.
 * \param ft The type of the field.
 * \param values The valid values for this field.
 */
bf::type_field_set::type_field_set
( const std::string& name, field_type ft, const std::list<std::string>& values )
  : type_field( name, ft ), m_values(values)
{

} // type_field_set::type_field_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a copy of this instance.
 */
bf::type_field* bf::type_field_set::clone() const
{
  return new type_field_set(*this);
} // type_field_set::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the type of the range of valid values.
 */
bf::type_field::range_type bf::type_field_set::get_range_type() const
{
  return field_range_set;
} // type_field_set::get_range_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the set of valid values for this field.
 * \param values (out) The valid values.
 */
void bf::type_field_set::get_set( std::list<std::string>& values ) const
{
  values = m_values;
} // type_field_set::get_set()
