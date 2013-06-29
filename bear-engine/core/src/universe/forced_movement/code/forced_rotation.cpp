/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::forced_rotation class.
 * \author Julien Jorge
 */
#include "universe/forced_movement/forced_rotation.hpp"

#include "universe/physical_item.hpp"

#include <cmath>
#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::forced_rotation::forced_rotation()
  : m_start_angle(-3.14159), m_end_angle(3.14159), m_angle(0),
    m_radius( std::numeric_limits<double>::infinity() ),
    m_total_time( std::numeric_limits<time_type>::infinity() ),
    m_elapsed_time(0), m_loop_back(false), m_loops(0), m_loop_count(0),
    m_angle_application(apply_keep)
{

} // forced_rotation::forced_rotation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this movement.
 */
bear::universe::base_forced_movement*
bear::universe::forced_rotation::clone() const
{
  return new forced_rotation(*this);
} // forced_rotation::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the radius of the rotation.
 * \param radius The radius.
 *
 * Give an infinite radius if you want it to be computed automatically at
 * initialization time.
 */
void bear::universe::forced_rotation::set_radius( double radius )
{
  m_radius = radius;
} // forced_rotation::set_radius()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the initial angle of the rotation.
 * \param start_angle The initial angle.
 */
void bear::universe::forced_rotation::set_start_angle( double start_angle )
{
  m_start_angle = start_angle;
} // forced_rotation::set_start_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the final angle of the rotation.
 * \param end_angle The final angle.
 */
void bear::universe::forced_rotation::set_end_angle( double end_angle )
{
  m_end_angle = end_angle;
} // forced_rotation::set_end_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell how the angle of the rotation is applied to the rotating item.
 * \param a The application mode.
 */
void
bear::universe::forced_rotation::set_angle_application( angle_application a )
{
  m_angle_application = a;
} // forced_rotation::set_angle_application()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell how long it takes to go from the initial angle to the last angle.
 * \param length The duration of the movement from the initial angle to the last
 *        angle.
 */
void bear::universe::forced_rotation::set_total_time( time_type length )
{
  m_total_time = length;
} // forced_rotation::set_total_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the duration of the acceleration and deceleration.
 * \param length The duration of the acceleration and deceleration.
 */
void bear::universe::forced_rotation::set_acceleration_time( time_type length )
{
  m_speed_generator.set_acceleration_time(length);
} // forced_rotation::set_acceleration_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell how many times the movement from the initial angle to the last
 *        angle must be done.
 * \param loops The number of loops.
 */
void bear::universe::forced_rotation::set_loops( unsigned int loops )
{
  m_loops = loops;
} // forced_rotation::set_loops()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we go backward when the final angle is reached.
 * \param loop_back Go backward.
 */
void bear::universe::forced_rotation::set_loop_back( bool loop_back )
{
  m_loop_back = loop_back;
} // forced_rotation::set_loop_back()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the movement is finished.
 */
bool bear::universe::forced_rotation::is_finished() const
{
  return (m_loop_count == m_loops) && m_loops;
} // forced_rotation::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the movement.
 */
void bear::universe::forced_rotation::do_init()
{
  if ( m_radius == std::numeric_limits<double>::infinity() )
    if ( has_reference_point() )
      m_radius =
        get_reference_position().distance( get_moving_item_position() );

  m_speed_generator.set_distance(m_end_angle - m_start_angle);
  m_speed_generator.set_total_time(m_total_time);

  if ( m_speed_generator.get_acceleration_time() > m_total_time )
    m_speed_generator.set_acceleration_time( m_total_time / 2 );

  m_angle = m_start_angle;
  m_loop_count = 0;
  m_is_playing_forward = true;

  set_item_position();
} // forced_rotation::do_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do an iteration of the movement.
 * \param elapsed_time Elapsed time since the last call.
 * \return The remaining time if the movement is finished.
 */
bear::universe::time_type
bear::universe::forced_rotation::do_next_position( time_type elapsed_time )
{
  if ( is_finished() )
    return elapsed_time;

  const time_type remaining_time = update_angle(elapsed_time);
  set_item_position();

  return remaining_time;
} // forced_rotation::do_next_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the next angle.
 * \param elapsed_time Elapsed time since the last call.
 * \return The remaining time if the movement is finished.
 */
bear::universe::time_type
bear::universe::forced_rotation::update_angle( const time_type elapsed_time )
{
  time_type remaining_time = 0;
  time_type t = elapsed_time;
  bool adjust(false);

  if ( m_elapsed_time + t > m_total_time )
    {
      remaining_time = m_elapsed_time + t - m_total_time;
      t = m_total_time - m_elapsed_time;
      adjust = true;
    }

  if ( m_total_time == 0 )
    m_angle = m_end_angle;
  else
    {
      m_elapsed_time += t;

      const double angular_velocity
        ( m_speed_generator.get_speed(m_elapsed_time) );
      m_angle += angular_velocity * t;
    }

  // movement is finished
  if ( adjust )
    {
      if ( m_is_playing_forward )
        end_reached();
      else
        start_reached();

      if ( (remaining_time > 0) && !is_finished()
           && (remaining_time != elapsed_time) )
        remaining_time = update_angle(remaining_time);
    }

  return remaining_time;
} // forced_rotation::update_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief The rotation reached the final angle. Loop back or stop.
 */
void bear::universe::forced_rotation::end_reached()
{
  m_elapsed_time = 0;

  if (m_loop_back)
    {
      m_speed_generator.set_distance( -m_speed_generator.get_distance() );
      m_angle = m_end_angle;
      m_is_playing_forward = false;
    }
  else
    {
      ++m_loop_count;

      if ( m_loop_count == m_loops )
        m_angle = m_end_angle;
      else
        m_angle = m_start_angle;
    }
} // forced_rotation::end_reached()

/*----------------------------------------------------------------------------*/
/**
 * \brief The rotation reached the initial angle. Loop.
 */
void bear::universe::forced_rotation::start_reached()
{
  ++m_loop_count;
  m_elapsed_time = 0;
  m_angle = m_start_angle;
  m_speed_generator.set_distance( -m_speed_generator.get_distance() );
  m_is_playing_forward = true;
} // forced_rotation::start_reached()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of the moving item.
 */
void bear::universe::forced_rotation::set_item_position()
{
  if ( has_reference_point() )
    {
      position_type pos;
      pos = get_reference_position();

      pos.x += (m_radius * cos( m_angle ));
      pos.y += (m_radius * sin( m_angle ));

      set_moving_item_position(pos);
    }

  switch (m_angle_application)
    {
    case apply_add:
      set_moving_item_system_angle( get_moving_item_system_angle() + m_angle );
      break;
    case apply_force:
      set_moving_item_system_angle( m_angle );
      break;
    case apply_force_delta:
      set_moving_item_system_angle( m_angle - m_start_angle );
      break;
    case apply_force_remaining:
      set_moving_item_system_angle( m_angle - m_end_angle );
      break;
    case apply_keep:
      { /* nothing to do */ }
    }
} // forced_rotation::set_item_position()
