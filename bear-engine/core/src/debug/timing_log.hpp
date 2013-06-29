/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class keep traces of different durations and output the total
 *        and average durations.
 * \author Julien Jorge
 */
#ifndef __DEBUG_TIMING_LOG_HPP__
#define __DEBUG_TIMING_LOG_HPP__

#include "time/time.hpp"

#include "debug/class_export.hpp"

#include <string>
#include <list>

namespace bear
{
  namespace debug
  {
    /**
     * \brief This class keep traces of different durations and output the total
     *        and average durations.
     * \author Julien Jorge
     */
    class DEBUG_EXPORT timing_log
    {
    public:
      timing_log
      ( const std::string& name, systime::milliseconds_type avg_interval );
      ~timing_log();

      void start();
      void stop();

    private:
      /** \brief The durations in m_avg_interval interval of time. */
      std::list<double> m_duration;

      /** \brief The name of this timer. */
      const std::string m_name;

      /** \brief The last time saved by a call to start(). */
      systime::milliseconds_type m_last_date;

      /** \brief The count of measures since the begining. */
      unsigned int m_duration_counter;

      /** \brief The sum of measures since the begining. */
      systime::milliseconds_type m_duration_sum;

      /** \brief The duration of the time intervals in which we compute the
          average duration. */
      systime::milliseconds_type m_avg_interval;

      /** \brief The date of the last computation of an average time. */
      systime::milliseconds_type m_last_avg;

      /** \brief The count of measures since the last computation of an average
          time. */
      unsigned int m_avg_counter;

      /** \brief The sum of measures since the last computation of an average
          time. */
      systime::milliseconds_type m_avg_sum;

    }; // class timing_log
  } // namespace debug
} // namespace bear

#ifdef BEAR_PROFILE
  /**
   * \brief Create a static timer.
   * \param name The name of the timer.
   * \param d The duration of the time window in which we compute the average
   *        duration.
   */
  #define BEAR_CREATE_TIMELOG( name, d )                            \
    static bear::debug::timing_log name ## _time_log( #name, d )

  /**
   * \brief Start a timer with a given name.
   * \param name The name of the timer.
   */
  #define BEAR_START_TIMELOG( name ) name ## _time_log.start()

  /**
   * \brief Stop a timer with a given name.
   * \param name The name of the timer.
   */
  #define BEAR_STOP_TIMELOG( name ) name ## _time_log.stop()
#else // def BEAR_PROFILE
  #define BEAR_CREATE_TIMELOG( name, d )
  #define BEAR_START_TIMELOG( name )
  #define BEAR_STOP_TIMELOG( name )
#endif // def BEAR_PROFILE

#endif // __DEBUG_TIMING_LOG_HPP__
