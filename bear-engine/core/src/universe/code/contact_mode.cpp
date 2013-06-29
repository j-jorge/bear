/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::contact_mode class.
 * \author Julien Jorge
 */
#include "universe/contact_mode.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value type corresponding to a given string representation.
 * \param v The string from which we find the value_type
 */
bear::universe::contact_mode::value_type
bear::universe::contact_mode::from_string( const std::string& v )
{
  if ( v == "full_contact" )
    return full_contact;
  else if ( v == "range_contact" )
    return range_contact;
  else
    return no_contact;
} // contact_mode::from_string()
