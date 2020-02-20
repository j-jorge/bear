/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::simple_edit class.
 * \author Julien Jorge
 */

#include "bf/stream_conv.hpp"
#include "bf/wx_facilities.hpp"

#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor from a value.
 * \param v The value.
 */
template<typename Type>
bf::simple_edit<Type>::simple_edit( const value_type& v )
  : super(v)
{

} // simple_edit::simple_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Transform the value in a string.
 */
template<typename Type>
wxString bf::simple_edit<Type>::value_to_string() const
{
  std::ostringstream oss;

  oss << this->get_value();

  return std_to_wx_string( oss.str() );
} // simple_edit::value_to_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Transform a string in a value.
 * \param str The string in which we read the value.
 * \return true If the string has been converted.
 */
template<typename Type>
bool bf::simple_edit<Type>::value_from_string( const wxString& str )
{
  std::istringstream iss( wx_to_std_string(str) );

  value_type v;
  bool result = true;

  if ( stream_conv<Type>::read(iss, v) )
    this->set_value(v);
  else
    result = false;

  return result;
} // simple_edit::value_from_string()
