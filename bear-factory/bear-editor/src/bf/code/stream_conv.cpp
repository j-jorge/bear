/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation the the specialisations of the bf::stream_conv class.
 * \author Julien Jorge
 */
#include "bf/stream_conv.hpp"

#include "bf/wx_facilities.hpp"
#include <wx/intl.h>
#include <claw/string_algorithm.hpp>
#include <vector>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the value from a stream.
 * \param is The stream in which we read.
 * \param v The value we have read.
 */
std::istream&
bf::stream_conv<bf::string_type>::read( std::istream& is, value_type& v )
{
  value_type::value_type real_v;

  // an empty string is ok
  if ( is.rdbuf()->in_avail() == 0 )
    v.set_value(real_v);
  else if ( std::getline(is, real_v) )
    v.set_value(real_v);

  return is;
} // read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the value from a stream.
 * \param is The stream in which we read.
 * \param v The value we have read.
 */
std::istream&
bf::stream_conv< bf::custom_type<bool> >::read
( std::istream& is, value_type& v )
{
  std::string t;
  bool result = false;

  // an empty string is ok
  if ( std::getline(is, t) )
    if ( (t == "1") || (t == "true")
         || (std_to_wx_string(t) == _("true")) )
      result = true;

  v.set_value(result);

  return is;
} // read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the value from a stream.
 * \param is The stream in which we read.
 * \param v The value we have read.
 */
std::istream&
bf::stream_conv< bf::color >::read
( std::istream& is, value_type& v )
{
  std::string t;
  color result;

  // an empty string is ok
  while ( std::getline(is, t, ',') )
    {
      std::vector<std::string> sp;

      claw::text::split(sp, t, '=');

      if (sp.size() != 1)
        {
          double d;
          std::istringstream iss( sp.back() );
          iss >> d;

          if ( sp.front() == " red" )
            result.set_intensity
              ( d, result.get_green_intensity(), result.get_blue_intensity() );
          else if ( sp.front() == " green" )
            result.set_intensity
              ( result.get_red_intensity(), d, result.get_blue_intensity() );
          else if ( sp.front() == " blue" )
            result.set_intensity
              ( result.get_red_intensity(), result.get_green_intensity(), d );
          else
            result.set_opacity( d );
        }      
    }

  v = result;

  return is;
} // read()
