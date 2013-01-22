/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::human_readable class.
 * \author Julien Jorge
 */

#include "bf/wx_facilities.hpp"

#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a value in a string representation.
 * \param v The value to transform.
 */
template<typename Type>
wxString bf::human_readable<Type>::convert( const value_type& v )
{
  std::ostringstream oss;

  oss << v;

  return std_to_wx_string(oss.str());
} // human_readable::convert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a value in a string representation.
 * \param v The value to transform.
 */
template<typename T>
wxString bf::human_readable< std::list<T> >::convert( const value_type& v )
{
  wxString result;

  result = wxT("[");

  if ( !v.empty() )
    {
      typename value_type::const_iterator it = v.begin();
      result += human_readable<T>::convert(*it);

      for (++it; it!=v.end(); ++it)
        result += wxT(", ") + human_readable<T>::convert(*it);
    }

  result += wxT("]");

  return result;
} // human_readable::convert() [list]
