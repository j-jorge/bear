/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the free functions of the namespace bear::systime.
 * \author Julien Jorge.
 */
#include "bear/time/time.hpp"

#include <ctime>
#include <locale>
#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the unix date, in seconds since January 1, 1970.
 */
bear::systime::seconds_type TIME_EXPORT bear::systime::get_unix_time()
{
  std::time_t t = std::time(NULL);
  return static_cast<seconds_type>( t );
} // get_unix_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a string representation of a time in seconds according to a given
 *        format.
 * \param t The time.
 * \param format The format.
 * \sa std::time_put.
 */
std::string TIME_EXPORT bear::systime::format_time_s
( unsigned long t, const std::string& format )
{
  std::locale loc;
  const std::time_put<char>& tmput =
    std::use_facet< std::time_put<char> >(loc);
  std::tm date;
  std::ostringstream oss;

  date.tm_sec = t % 60;
  t /= 60.;

  date.tm_min = t % 60;
  t /= 60.;

  date.tm_hour = t;

  tmput.put
    ( oss, oss, ' ', &date, format.c_str(), format.c_str() + format.length() );

  return oss.str();
} // format_time()
