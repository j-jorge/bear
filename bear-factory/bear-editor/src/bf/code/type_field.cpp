/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::type_field class.
 * \author Julien Jorge
 */
#include "bf/type_field.hpp"
#include <iostream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the field.
 * \param ft The type of the field.
 */
bf::type_field::type_field( const std::string& name, field_type ft )
  : m_name(name), m_field_type(ft), m_required(false), m_is_list(false)
{

} // type_field::type_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::type_field::~type_field()
{

} // type_field::~type_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a copy of this instance.
 */
bf::type_field* bf::type_field::clone() const
{
  return new type_field(*this);
} // type_field::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the typname of the field.
 */
const std::string& bf::type_field::get_name() const
{
  return m_name;
} // type_field::get_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the type of the field.
 */
bf::type_field::field_type bf::type_field::get_field_type() const
{
  return m_field_type;
} // type_field::get_field_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the type of the range of valid values.
 */
bf::type_field::range_type bf::type_field::get_range_type() const
{
  return field_range_free;
} // type_field::get_range_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the set of valid values for this field.
 * \param values (out) The valid values.
 */
void bf::type_field::get_set( std::list<std::string>& values ) const
{
  // nothing to do
} // type_field::get_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the interval of valid values for this field.
 * \param min Minimum valid value.
 * \param max Maximum valid value.
 */
void bf::type_field::get_interval( int& min, int& max ) const
{
  min = max = 0;
} // type_field::get_interval()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the interval of valid values for this field.
 * \param min Minimum valid value.
 * \param max Maximum valid value.
 */
void bf::type_field::get_interval( unsigned int& min, unsigned int& max ) const
{
  min = max = 0;
} // type_field::get_interval()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the interval of valid values for this field.
 * \param min Minimum valid value.
 * \param max Maximum valid value.
 */
void bf::type_field::get_interval( double& min, double& max ) const
{
  min = max = 0;
} // type_field::get_interval()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the fields that must be defined before this one.
 */
const std::set<std::string>& bf::type_field::get_preceding() const
{
  return m_preceding;
} // type_field::get_preceding()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the names of the fields that must be defined before this one.
 * \param prec The names of the fields that must be defined before this one.
 */
void bf::type_field::set_preceding( const std::list<std::string>& prec )
{
  m_preceding.insert(prec.begin(), prec.end());
} // type_field::set_preceding()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this field must be filled.
 * \param r Required or not?
 */
void bf::type_field::set_required( bool r )
{
  m_required = r;
} // type_field::set_required()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this field is required or optional.
 */
bool bf::type_field::get_required() const
{
  return m_required;
} // type_field::get_required()

/*----------------------------------------------------------------------------*/
/**
 * \brief Give a description to this node.
 * \param str The description.
 */
void bf::type_field::set_description( const std::string& str )
{
  m_description = str;
} // type_field::set_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the description of this field.
 */
const std::string& bf::type_field::get_description() const
{
  return m_description;
} // type_field::get_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Give the string representation of the default value of the field.
 * \param str The value.
 */
void bf::type_field::set_default_value( const std::string& str )
{
  m_default_value = str;
} // type_field::set_default_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the string representation of the default value of this field.
 */
const std::string& bf::type_field::get_default_value() const
{
  return m_default_value;
} // type_field::get_default_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this field is a list of values.
 * \param v List or not?
 */
void bf::type_field::set_is_list( bool v )
{
  m_is_list = v;
} // type_field::set_is_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this field is a list of values.
 */
bool bf::type_field::is_list() const
{
  return m_is_list;
} // type_field::is_list()
