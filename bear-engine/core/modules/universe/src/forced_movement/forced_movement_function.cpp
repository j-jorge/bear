/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::forced_movement_function class.
 * \author Julien Jorge
 */
#include "bear/universe/forced_movement/forced_movement_function.hpp"

#include "bear/universe/physical_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The function to call to get the position of the item.
 * \param length The duration of the movement.
 */
bear::universe::forced_movement_function::forced_movement_function
( const function_type& f, time_type length )
  : m_total_time( length ), m_remaining_time(m_total_time), m_function(f)
{

} // forced_movement_function::forced_movement_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this movement.
 */
bear::universe::base_forced_movement*
bear::universe::forced_movement_function::clone() const
{
  return new forced_movement_function(*this);
} // forced_movement_function::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the duration of the movement.
 * \param length The duration of the movement.
 */
void
bear::universe::forced_movement_function::set_total_time( time_type length )
{
  m_total_time = length;
} // forced_movement_function::set_total_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the movement is finished.
 */
bool bear::universe::forced_movement_function::is_finished() const
{
  return m_remaining_time == 0;
} // forced_movement_function::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the movement.
 */
void bear::universe::forced_movement_function::do_init()
{
  m_remaining_time = m_total_time;
} // forced_movement_function::do_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do an iteration of the movement.
 * \param elapsed_time Elapsed time since the last call.
 * \return The remaining time if the movement is finished.
 */
bear::universe::time_type
bear::universe::forced_movement_function::do_next_position
( time_type elapsed_time )
{
  time_type result;

  if ( m_remaining_time < elapsed_time )
    {
      result = elapsed_time - m_remaining_time;
      m_remaining_time = 0;
    }
  else
    {
      m_remaining_time -= elapsed_time;
      result = 0;
    }

  get_item().set_center_of_mass( m_function() );

  return result;
} // forced_movement_function::do_next_position()
