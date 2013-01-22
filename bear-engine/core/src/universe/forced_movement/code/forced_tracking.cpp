/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::forced_tracking class.
 * \author Julien Jorge
 */
#include "universe/forced_movement/forced_tracking.hpp"

#include "universe/physical_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param length The duration of the movement.
 */
bear::universe::forced_tracking::forced_tracking( time_type length )
  : m_distance( std::numeric_limits<coordinate_type>::infinity(),
                std::numeric_limits<coordinate_type>::infinity() ),
    m_total_time(length), m_remaining_time(m_total_time)
{

} // forced_tracking::forced_tracking()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param distance The distance to maintain.
 * \param length The duration of the movement.
 */
bear::universe::forced_tracking::forced_tracking
( const position_type& distance, time_type length )
  : m_distance(distance), m_total_time(length), m_remaining_time(m_total_time)
{

} // forced_tracking::forced_tracking()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this movement.
 */
bear::universe::base_forced_movement*
bear::universe::forced_tracking::clone() const
{
  return new forced_tracking(*this);
} // forced_tracking::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the distance to maintain.
 * \param distance The distance to maintain.
 */
void
bear::universe::forced_tracking::set_distance( const position_type& distance )
{
  m_distance = distance;
} // forced_tracking::set_distance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the duration of the movement.
 * \param length The duration of the movement.
 */
void bear::universe::forced_tracking::set_total_time( time_type length )
{
  m_total_time = length;
} // forced_tracking::set_total_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the distance to maintain.
 */
const bear::universe::position_type&
bear::universe::forced_tracking::get_distance() const
{
  return m_distance;
} // forced_tracking::get_distance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the movement is finished.
 */
bool bear::universe::forced_tracking::is_finished() const
{
  return !has_reference_point() || (m_remaining_time == 0);
} // forced_tracking::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the movement.
 */
void bear::universe::forced_tracking::do_init()
{
  m_remaining_time = m_total_time;

  if ( has_reference_point() )
    {
      if ( m_distance.x == std::numeric_limits<coordinate_type>::infinity() )
        m_distance.x =
          get_moving_item_position().x - get_reference_position().x;

      if ( m_distance.y == std::numeric_limits<coordinate_type>::infinity() )
        m_distance.y =
          get_moving_item_position().y - get_reference_position().y;
    }

  next_position(0);
} // forced_tracking::do_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do an iteration of the movement.
 * \param elapsed_time Elapsed time since the last call.
 * \return The remaining time if the movement is finished.
 */
bear::universe::time_type
bear::universe::forced_tracking::do_next_position( time_type elapsed_time )
{
  time_type result(0);

  if ( has_reference_point() )
    {
      set_moving_item_position( get_reference_position() + m_distance );

      if ( m_remaining_time < elapsed_time )
        {
          result = elapsed_time - m_remaining_time;
          m_remaining_time = 0;
        }
      else
        m_remaining_time -= elapsed_time;
    }

  return result;
} // forced_tracking::do_next_position()
