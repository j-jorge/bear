/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::custom_type class.
 * \author Julien Jorge
 */
#include "bf/custom_type.hpp"

#include "bf/compiled_file.hpp"
#include "bf/path_configuration.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Output the value in the compiled file.
 * \param cf The file to write in.
 */
void bf::base_file_type::compile( compiled_file& cf ) const
{
  std::string p(get_value());

  if ( path_configuration::get_instance().expand_file_name(p) )
    path_configuration::get_instance().get_relative_path(p);

  cf << p;
} // base_file_type::compile()




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
