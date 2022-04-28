/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Unix implementation of the functions of the namespace bear::systime.
 * \author Julien Jorge.
 */
#include "bear/time/time.hpp"

#include <unistd.h>
#include <cstddef>
#include <sys/time.h>
#include <time.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop the process for a given amount of time.
 * \param ms Stop the process this number of milliseconds.
 */
void bear::systime::sleep( milliseconds_type ms )
{
  struct timespec time;
  time.tv_sec = ms / 1000;
  time.tv_nsec = ( ms % 1000 ) * ( 1000 * 1000 );
  nanosleep( &time, NULL );
} // sleep()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current date, in milliseconds.
 */
bear::systime::milliseconds_type bear::systime::get_date_ms()
{
  timeval tv;

  gettimeofday( &tv, NULL );

  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
} // get_date_ms()
