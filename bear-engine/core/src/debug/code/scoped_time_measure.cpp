/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::debug::scoped_time_measure class.
 * \author Julien Jorge
 */
#include "debug/scoped_time_measure.hpp"

#include "debug/timelog.hpp"

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the timer.
 */
bear::debug::scoped_time_measure::scoped_time_measure
( const std::string& name )
  : m_name( timelog::format_key(name) ), m_start_date( systime::get_date_ms() )
{

} // scoped_time_measure::scoped_time_measure()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::debug::scoped_time_measure::~scoped_time_measure()
{
#ifdef __ANDROID__
  claw::logger << claw::log_verbose << "[PROF]" << m_name << '\t'
                        << systime::get_date_ms() - m_start_date
                        << '\n';
#else
  timelog::get_output() << m_name << '\t'
                        << systime::get_date_ms() - m_start_date
                        << '\n';
#endif
} // scoped_time_measure::~scoped_time_measure()
