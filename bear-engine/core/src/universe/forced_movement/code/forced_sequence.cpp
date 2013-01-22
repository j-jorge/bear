/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::forced_sequence class.
 * \author Julien Jorge
 */
#include "universe/forced_movement/forced_sequence.hpp"

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::forced_sequence::forced_sequence()
  : m_index(0), m_loops(0), m_play_count(0), m_finished(false),
    m_has_changed(false)
{

} // forced_sequence::forced_sequence()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this movement.
 */
bear::universe::base_forced_movement*
bear::universe::forced_sequence::clone() const
{
  return new forced_sequence(*this);
} // forced_sequence::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a movement at the end of the sequence.
 * \param m The movement to add.
 */
void bear::universe::forced_sequence::push_back( const forced_movement& m )
{
  m_sub_sequence.push_back( m );
  m_sub_sequence.back().set_auto_remove(false);
} // forced_sequence::push_back()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell how many times the sequence must be done.
 * \param loops The number of loops.
 */
void bear::universe::forced_sequence::set_loops( unsigned int loops )
{
  m_loops = loops;
} // forced_sequence::set_loops()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the movement is finished.
 */
bool bear::universe::forced_sequence::is_finished() const
{
  return ( (m_play_count == m_loops) && m_loops ) || m_finished;
} // forced_sequence::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the movement.
 */
void bear::universe::forced_sequence::do_init()
{
  m_play_count = 0;
  m_index = 0;
  m_has_changed = false;
  m_finished = false;

  for ( unsigned int i = 0; i != m_sub_sequence.size(); ++i )
    {
      m_sub_sequence[i].set_moving_item_gap( get_moving_item_gap() );
      m_sub_sequence[i].set_moving_item_ratio( get_moving_item_ratio() );
    }

  if ( !m_sub_sequence.empty() )
    init_current_subsequence();
} // forced_sequence::do_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do an iteration of the movement.
 * \param elapsed_time Elapsed time since the last call.
 * \return The remaining time if the movement is finished.
 */
bear::universe::time_type
bear::universe::forced_sequence::do_next_position( time_type elapsed_time )
{
  if ( is_finished() )
    return elapsed_time;

  time_type remaining_time(elapsed_time);

  if ( !m_sub_sequence.empty() )
    {
      remaining_time = m_sub_sequence[m_index].next_position(remaining_time);

      if ( m_sub_sequence[m_index].is_finished() )
        {
          next_sequence();

          m_has_changed = m_has_changed || (remaining_time != elapsed_time);

          if ( remaining_time > 0 )
            if ( !is_finished() )
              remaining_time = next_position(remaining_time);
        }
    }

  return remaining_time;
} // forced_sequence::do_next_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the next sub sequence.
 */
void bear::universe::forced_sequence::next_sequence()
{
  m_sub_sequence[m_index].clear_item();
  ++m_index;

  if ( m_index == m_sub_sequence.size() )
    {
      ++m_play_count;
      m_index = 0;

      if ( !m_has_changed && (m_loops == 0) )
        {
          claw::logger << claw::log_warning
                       << "forced_sequence loop has no effect. Stopping."
                       << std::endl;
          m_finished = true;
        }
      else
        m_has_changed = false;
    }

  if ( !is_finished() )
    init_current_subsequence();
} // forced_sequence::next_sequence()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initializes the current subsequence. The item of this movement is
 *        passed to it and the reference item of the subsequence, if any, is
 *        passed to this.
 */
void bear::universe::forced_sequence::init_current_subsequence()
{
  m_sub_sequence[m_index].set_item( get_item() );
  m_sub_sequence[m_index].init();

  if ( m_sub_sequence[m_index].has_reference_item() )
    set_reference_point_on_center
      ( m_sub_sequence[m_index].get_reference_item() );
} // forced_sequence::init_current_subsequence()
