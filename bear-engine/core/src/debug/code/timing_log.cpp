/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::debug::timing_log class.
 * \author Julien Jorge
 */
#include "debug/timing_log.hpp"

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the timer.
 * \param avg_interval The duration of the time window in which we compute the
 *        average duration.
 */
bear::debug::timing_log::timing_log
( const std::string& name, systime::milliseconds_type avg_interval )
  : m_name(name), m_duration_counter(0), m_duration_sum(0),
    m_avg_interval(avg_interval), m_avg_counter(0), m_avg_sum(0)
{

} // timing_log::timing_log()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructior.
 */
bear::debug::timing_log::~timing_log()
{
  if ( !m_duration.empty() )
    {
      std::list<double>::const_iterator it;
      std::ofstream f( (m_name + ".timelog").c_str() );
      unsigned int i = 0;

      for (it=m_duration.begin(); it!=m_duration.end(); ++it, ++i)
        f << i << ' ' << *it << '\n';
    }

  claw::logger << claw::log_verbose << "timelog " << m_name << " total="
               << m_duration_sum << " ms, avg="
               << ((double)m_duration_sum / (double)m_duration_counter)
               << " ms." << std::endl;
} // timing_log::~timing_log()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start to measure something.
 */
void bear::debug::timing_log::start()
{
  m_last_date = systime::get_date_ms();
} // timing_log::start()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop the last measure.
 */
void bear::debug::timing_log::stop()
{
  systime::milliseconds_type current_time = systime::get_date_ms();

  m_duration_sum += current_time - m_last_date;
  ++m_duration_counter;

  if (m_avg_counter == 0)
    m_last_avg = m_last_date;

  m_avg_sum += current_time - m_last_date;
  ++m_avg_counter;

  if ( current_time - m_last_avg >= m_avg_interval )
    {
      m_duration.push_back( (double)m_avg_sum / (double)m_avg_counter );
      m_avg_sum = 0;
      m_avg_counter = 0;
    }
} // timing_log::stop()
