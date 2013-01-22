/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::forced_goto class.
 * \author Julien Jorge.
 */
#include "universe/forced_movement/forced_goto.hpp"

#include "universe/physical_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param length Maximum time to reach the target.
 */
bear::universe::forced_goto::forced_goto( time_type length )
  : m_total_time(length), m_elapsed_time(0), m_target_position(0, 0),
    m_length(0, 0)
{

} // forced_goto::forced_goto()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this movement.
 */
bear::universe::base_forced_movement* bear::universe::forced_goto::clone() const
{
  return new forced_goto(*this);
} // forced_goto::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the total time to reach the target position.
 * \param length Total time to reach the target position.
 */
void bear::universe::forced_goto::set_total_time( time_type length )
{
  m_total_time = length;
} // forced_goto::set_total_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the duration of the acceleration and deceleration.
 * \param length The duration of the acceleration and deceleration.
 */
void bear::universe::forced_goto::set_acceleration_time( time_type length )
{
  m_speed_generator.set_acceleration_time(length);
} // forced_goto::set_acceleration_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the length of the movement.
 * \param v The length of the movement.
 */
void bear::universe::forced_goto::set_length( const vector_type& v )
{
  set_x_length(v.x);
  set_y_length(v.y);
} // forced_goto::set_length()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the X-length of the movement.
 * \param v The length.
 */
void bear::universe::forced_goto::set_x_length( coordinate_type v )
{
  m_length.x = v;
} // forced_goto::set_x_length()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the Y-length of the movement.
 * \param v The length.
 */
void bear::universe::forced_goto::set_y_length( coordinate_type v )
{
  m_length.y = v;
} // forced_goto::set_y_length()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the movement is finished.
 */
bool bear::universe::forced_goto::is_finished() const
{
  return m_elapsed_time == m_total_time;
} // forced_goto::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the movement.
 */
void bear::universe::forced_goto::do_init()
{
  m_elapsed_time = 0;

  m_target_position = get_moving_item_position() + m_length;

  m_speed_generator.set_distance
    ( m_target_position.distance(get_moving_item_position()) );
  m_speed_generator.set_total_time(m_total_time);

  if ( m_speed_generator.get_acceleration_time() > m_total_time )
    m_speed_generator.set_acceleration_time( m_total_time / 2 );
} // forced_goto::do_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do an iteration of the movement.
 * \param elapsed_time Elapsed time since the last call.
 * \return The remaining time if the movement is finished.
 */
bear::universe::time_type
bear::universe::forced_goto::do_next_position( time_type elapsed_time )
{
  time_type remaining_time(0);

  if ( m_elapsed_time + elapsed_time >= m_total_time )
    {
      remaining_time = m_elapsed_time + elapsed_time - m_total_time;
      m_elapsed_time = m_total_time;
      set_moving_item_position(m_target_position);
    }
  else
    {
      vector_type d(m_length);
      d.normalize();

      set_moving_item_position
        ( get_moving_item_position()
          + m_speed_generator.get_speed(m_elapsed_time) * d * elapsed_time );

      m_elapsed_time += elapsed_time;
    }

  return remaining_time;
} // forced_goto::do_next_position()
