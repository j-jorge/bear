/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::sinus_speed_generator class.
 * \author Julien Jorge.
 */
#include "bear/universe/forced_movement/sinus_speed_generator.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::sinus_speed_generator::sinus_speed_generator()
  : m_total_time(std::numeric_limits<time_type>::infinity()),
    m_acceleration_time(m_total_time),
    m_distance(std::numeric_limits<coordinate_type>::infinity())
{

} // sinus_speed_generator::sinus_speed_generator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the total time of the movement.
 * \param length Total time of the movement.
 */
void bear::universe::sinus_speed_generator::set_total_time( time_type length )
{
  m_total_time = length;
} // sinus_speed_generator::set_total_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the total time of the movement.
 */
bear::universe::time_type
bear::universe::sinus_speed_generator::get_total_time() const
{
  return m_total_time;
} // sinus_speed_generator::get_total_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the duration of the acceleration and deceleration.
 * \param length The duration of the acceleration and deceleration.
 */
void
bear::universe::sinus_speed_generator::set_acceleration_time( time_type length )
{
  m_acceleration_time = length;
} // sinus_speed_generator::set_acceleration_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the duration of the acceleration and deceleration.
 */
bear::universe::time_type
bear::universe::sinus_speed_generator::get_acceleration_time() const
{
  return m_acceleration_time;
} // sinus_speed_generator::get_acceleration_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the distance of the journey.
 * \param d The distance
 */
void bear::universe::sinus_speed_generator::set_distance( coordinate_type d )
{
  m_distance = d;
} // sinus_speed_generator::set_distance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the distance of the journey.
 */
bear::universe::coordinate_type
bear::universe::sinus_speed_generator::get_distance() const
{
  return m_distance;
} // sinus_speed_generator::get_distance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the speed at a given time.
 * \param t The date at which we want the speed.
 */
bear::universe::coordinate_type
bear::universe::sinus_speed_generator::get_speed( time_type t ) const
{
  const coordinate_type vmax =
    m_distance / ( m_total_time - m_acceleration_time );
  coordinate_type result;

  if ( t < m_acceleration_time )
    result = (-std::cos(t * 3.14159 / m_acceleration_time) + 1) * vmax / 2;
  else if ( t > m_total_time - m_acceleration_time )
    {
      t -= m_total_time - m_acceleration_time;
      result = (std::cos(t * 3.14159 / m_acceleration_time) + 1) * vmax / 2;
    }
  else
    result = vmax;

  return result;
} // sinus_speed_generator::get_speed()
