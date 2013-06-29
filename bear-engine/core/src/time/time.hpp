/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Some functions and types related to time management.
 * \author Julien Jorge.
 */
#ifndef __TIME_TIME_HPP__
#define __TIME_TIME_HPP__

#include "time/class_export.hpp"

#include <string>

namespace bear
{
  namespace systime
  {
    typedef unsigned long milliseconds_type;
    typedef unsigned long seconds_type;

    void TIME_EXPORT sleep( milliseconds_type ms );
    milliseconds_type TIME_EXPORT get_date_ms();

    seconds_type TIME_EXPORT get_unix_time();

    std::string TIME_EXPORT format_time_s
    ( unsigned long t, const std::string& format );

  } // namespace systime
} // namespace bear

#endif // __TIME_TIME_HPP__
