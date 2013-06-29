/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class measures the time spent between its creation and its
 *        destruction.
 * \author Julien Jorge
 */
#ifndef __DEBUG_SCOPED_TIME_MEASURE_HPP__
#define __DEBUG_SCOPED_TIME_MEASURE_HPP__

#include "time/time.hpp"

#include "debug/class_export.hpp"

#include <string>
#include <list>

namespace bear
{
  namespace debug
  {
    /**
     * \brief This class measures the time spent between its creation and its
     *        destruction.
     * \author Julien Jorge
     */
    class DEBUG_EXPORT scoped_time_measure
    {
    public:
      explicit scoped_time_measure( const std::string& name );
      ~scoped_time_measure();

    private:
      /** \brief The name of this measure. */
      const std::string m_name;

      /** \brief The date at which the measure started. */
      systime::milliseconds_type m_start_date;

    }; // class scoped_time_measure
  } // namespace debug
} // namespace bear

#ifdef BEAR_PROFILE
  /**
   * \brief Creates a scoped_time_measure.
   * \param name The name of the timer.
   */
  #define BEAR_CREATE_SCOPED_TIMELOG( name )                              \
    bear::debug::scoped_time_measure                                      \
    __FUNCTION__ ## __LINE__ ## _scoped_time_log( (name) )

#else // def BEAR_PROFILE
  #define BEAR_CREATE_SCOPED_TIMELOG( name )
#endif // def BEAR_PROFILE

#endif // __DEBUG_SCOPED_TIME_MEASURE_HPP__
