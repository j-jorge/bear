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
#include "bf/custom_type.hpp"

#include "bf/compilation_context.hpp"
#include "bf/compiled_file.hpp"
#include "bf/path_configuration.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Output the value in the compiled file.
 * \param cf The file to write in.
 * \param c The context in which the compilation is done.
 */
void bf::base_file_type::compile
( compiled_file& cf, compilation_context& c ) const
{
  std::string p(get_value());

  if ( path_configuration::get_instance().expand_file_name
       ( p, c.get_workspace_name() ) )
    path_configuration::get_instance().get_relative_path
      ( p, c.get_workspace_name() );

  cf << p;
} // base_file_type::compile()




/*----------------------------------------------------------------------------*/
/**
 * \brief Output the value in the compiled file.
 * \param cf The file to write in.
 */
void bf::easing_type::compile( compiled_file& cf ) const
{
  bear::easing e(get_value());

  cf << e.to_string();
} // easing_type::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two easing_type.
 * \param that The value to compare to.
 */
bool bf::easing_type::operator==( const easing_type& that) const
{
  return this->get_value() == that.get_value();
} // easing_type::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two easing_type.
 * \param that The value to compare to.
 */
bool bf::easing_type::operator!=( const easing_type& that) const
{
  return this->get_value() != that.get_value();
} // easing_type::operator!=()




/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two item_reference_type.
 * \param that The value to compare to.
 */
bool bf::item_reference_type::operator==( const item_reference_type& that) const
{
  return this->get_value() == that.get_value();
} // item_reference_type::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two item_reference_type.
 * \param that The value to compare to.
 */
bool bf::item_reference_type::operator!=( const item_reference_type& that) const
{
  return this->get_value() != that.get_value();
} // item_reference_type::operator!=()




/*----------------------------------------------------------------------------*/
/**
 * \brief Stream output operator.
 * \param os The stream in which we write.
 * \param v The value to write in the stream.
 */
std::ostream& operator<<( std::ostream& os, const bf::easing_type& v )
{
  return os << v.get_value().to_string();
} // operator<<()

