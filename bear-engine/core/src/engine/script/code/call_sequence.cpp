/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::call_sequence class.
 * \author Julien Jorge
 */
#include "engine/script/call_sequence.hpp"

#include "engine/script/method_call.hpp"

#include <claw/logger.hpp>
#include <algorithm>

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two calls according to their dates, in increasing order.
 * \param a The left operand.
 * \param b The right operand.
 * \return a.date < b.date
 */
bool bear::engine::call_sequence::call_info::compare_by_date::operator()
  ( const call_info& a, const call_info& b ) const
{
  return a.date < b.date;
} // call_sequence::call_info::compare_by_date()





/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param d The date at which the method has to be called.
 * \param c The data of the call.
 */
bear::engine::call_sequence::call_info::call_info
( universe::time_type d, const method_call& c )
  : date(d), call(c)
{

} // call_sequence::call_info::call_info()





/*----------------------------------------------------------------------------*/
/**
 * \brief Add a call to a method in the sequence.
 * \param date The date at which the method has to be called.
 * \param c The data of the call.
 */
void bear::engine::call_sequence::add_call
( universe::time_type date, const method_call& c )
{
  if ( m_sequence.empty() )
    m_sequence.push_back( call_info(date, c) );
  else
    {
      if ( m_sequence.back().date <= date )
        m_sequence.push_back( call_info(date, c) );
      else
        {
          claw::logger << claw::log_warning
                       << "The script is not ordered. You should consider to "
                       << "order the calls by increasing order of date."
                       << std::endl;

          m_sequence.push_back( call_info(date, c) );
          std::push_heap
            ( m_sequence.begin(), m_sequence.end(),
              call_info::compare_by_date() );
        }
    }
} // call_sequence::add_call()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first call.
 */
bear::engine::call_sequence::const_iterator
bear::engine::call_sequence::begin() const
{
  return m_sequence.begin();
} // call_sequence::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator just past the last call.
 */
bear::engine::call_sequence::const_iterator
bear::engine::call_sequence::end() const
{
  return m_sequence.end();
} // call_sequence::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the sequence is empty.
 */
bool bear::engine::call_sequence::empty() const
{
  return m_sequence.empty();
} // call_sequence::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell how many calls are in this sequence.
 */
std::size_t bear::engine::call_sequence::size() const
{
  return m_sequence.size();
} // call_sequence::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the date of the last entry.
 * \return The date of the last entry or zero if the sequence is empty.
 */
bear::universe::time_type bear::engine::call_sequence::get_last_date() const
{
  if ( m_sequence.empty() )
    return 0;
  else
    return m_sequence.back().date;
} // call_sequence::last_date()
