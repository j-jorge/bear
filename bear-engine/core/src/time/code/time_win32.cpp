/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief MS Windows implementation of the functions of the namespace
 *        bear::systime.
 * \author Julien Jorge.
 */
#include <windows.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop the process for a given amount of time.
 * \param ms Stop the process this number of milliseconds.
 */
void TIME_EXPORT bear::systime::sleep( milliseconds_type ms )
{
  Sleep( (DWORD)ms );
} // sleep()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current date, in milliseconds.
 */
bear::systime::milliseconds_type TIME_EXPORT bear::systime::get_date_ms()
{
  LARGE_INTEGER freq, c;

  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&c);

  return (milliseconds_type)((c.QuadPart * 1000) / freq.QuadPart);
} // get_date_ms()
