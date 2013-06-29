/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::forced_join class.
 * \author Julien Jorge.
 */
#include "universe/forced_movement/forced_join.hpp"

#include "universe/physical_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \steps length Maximum time to reach the target.
 */
bear::universe::forced_join::forced_join( time_type length )
  : m_total_time(length), m_remaining_time(m_total_time)
{

} // forced_join::forced_join()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this movement.
 */
bear::universe::base_forced_movement* bear::universe::forced_join::clone() const
{
  return new forced_join(*this);
} // forced_join::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the total time to reach the target.
 * \steps length Maximum time to reach the target.
 */
void bear::universe::forced_join::set_total_time( time_type length )
{
  m_total_time = length;
} // forced_join::set_total_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the movement is finished.
 */
bool bear::universe::forced_join::is_finished() const
{
  return !has_reference_point() || (m_remaining_time == 0);
} // forced_join::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the movement.
 */
void bear::universe::forced_join::do_init()
{
  m_remaining_time = m_total_time;
  m_initial_item_position = get_moving_item_position();
  m_initial_target_position = get_reference_position();

  next_position(0);
} // forced_join::do_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do an iteration of the movement.
 * \param elapsed_time Elapsed time since the last call.
 * \return The remaining time if the movement is finished.
 */
bear::universe::time_type
bear::universe::forced_join::do_next_position( time_type elapsed_time )
{
  time_type remaining_time(0);

  if ( has_reference_point() )
    {
      const position_type target_center = get_reference_position();
      const position_type fictive_item = 
        m_initial_item_position + target_center - m_initial_target_position;
      position_type dp = m_initial_target_position - m_initial_item_position;

      if ( m_remaining_time > elapsed_time )
        {
          dp = dp / m_total_time * ( m_total_time - m_remaining_time);
          m_remaining_time -= elapsed_time;
        }
      else
        remaining_time = elapsed_time - m_remaining_time;

      set_moving_item_position( fictive_item + dp );

      if ( (fictive_item + dp) == target_center )
        m_remaining_time = 0;
    }

  return remaining_time;
} // forced_join::do_next_position()
