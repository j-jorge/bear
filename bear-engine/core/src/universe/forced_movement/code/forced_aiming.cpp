/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::forced_aiming class.
 * \author Julien Jorge
 */
#include "universe/forced_movement/forced_aiming.hpp"

#include "universe/physical_item.hpp"

#include <cmath>
#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param length The duration of the movement.
 */
bear::universe::forced_aiming::forced_aiming( time_type length )
  : m_max_angle( std::numeric_limits<double>::infinity() ),
    m_max_speed( std::numeric_limits<double>::infinity() ),
    m_acceleration( std::numeric_limits<double>::infinity() ),
    m_total_time( length ), m_remaining_time(m_total_time), m_apply_angle(false)
{

} // forced_aiming::forced_aiming()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this movement.
 */
bear::universe::base_forced_movement*
bear::universe::forced_aiming::clone() const
{
  return new forced_aiming(*this);
} // forced_aiming::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the maximum angle when the direction changes.
 * \param a The angle.
 */
void bear::universe::forced_aiming::set_max_angle( double a )
{
  if (a < 0)
    m_max_angle = -a;
  else
    m_max_angle = a;
} // forced_aiming::set_max_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the maximum speed of the moving item.
 * \param s The speed.
 */
void bear::universe::forced_aiming::set_max_speed( double s )
{
  if( s < 0 )
    m_max_speed = -s;
  else
    m_max_speed = s;
} // forced_aiming::set_max_speed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the acceleration of the moving item.
 * \param a The acceleration.
 */
void bear::universe::forced_aiming::set_acceleration( double a )
{
  if( a < 0 )
    m_acceleration = -a;
  else
    m_acceleration = a;
} // forced_aiming::set_acceleration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we apply the angle of the movement to the item.
 * \param b Apply or not.
 */
void bear::universe::forced_aiming::set_apply_angle( bool b )
{
  m_apply_angle = b;
} // forced_aiming::set_apply_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the duration of the movement.
 * \param length The duration of the movement.
 */
void bear::universe::forced_aiming::set_total_time( time_type length )
{
  m_total_time = length;
} // forced_aiming::set_total_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the movement is finished.
 */
bool bear::universe::forced_aiming::is_finished() const
{
  return !has_reference_point() || (m_remaining_time == 0);
} // forced_aiming::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the movement.
 */
void bear::universe::forced_aiming::do_init()
{
  m_remaining_time = m_total_time;
} // forced_aiming::do_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do an iteration of the movement.
 * \param elapsed_time Elapsed time since the last call.
 * \return The remaining time if the movement is finished.
 */
bear::universe::time_type
bear::universe::forced_aiming::do_next_position( time_type elapsed_time )
{
  time_type remaining_time(0);

  if ( has_reference_point() )
    {
      remaining_time = compute_remaining_time(elapsed_time);

      double s = compute_speed(elapsed_time);

      vector_type dir;
      double a = compute_direction(dir);

      set_moving_item_position
        ( get_moving_item_position() + dir * s * elapsed_time );

      if ( m_apply_angle )
        set_moving_item_system_angle( a );
    }

  return remaining_time;
} // forced_aiming::do_next_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the remaining time after this step of the movement.
 * \param elapsed_time Elapsed time since the last call to do_next_position().
 */
bear::universe::time_type
bear::universe::forced_aiming::compute_remaining_time( time_type& elapsed_time )
{
  time_type result;

  if ( elapsed_time > m_remaining_time )
    {
      result = elapsed_time - m_remaining_time;
      elapsed_time = m_remaining_time;
      m_remaining_time = 0;
    }
  else
    {
      result = 0;
      m_remaining_time -= elapsed_time;
    }

  return result;
} // forced_aiming::compute_remaining_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the speed to apply to the item.
 * \param elapsed_time Elapsed time since the last call to do_next_position().
 */
double
bear::universe::forced_aiming::compute_speed( time_type elapsed_time ) const
{
  double s = get_item().get_speed().length();

  if ( s < m_max_speed )
    s = std::min(s + m_acceleration * elapsed_time, m_max_speed);
  else if ( s > m_max_speed )
    s = std::max(s - m_acceleration * elapsed_time, m_max_speed);

  return s;
} // forced_aiming::compute_speed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the direction to apply to the item.
 * \param dir (out) The direction.
 * \return The angle of the direction.
 */
double
bear::universe::forced_aiming::compute_direction( vector_type& dir ) const
{
  vector_type current_dir( get_item().get_speed() );

  dir = vector_type( get_moving_item_position(), get_reference_position() );

  current_dir.normalize();
  dir.normalize();

  if ( current_dir == vector_type(0, 0) )
    current_dir = dir;

  double v = dir.dot_product(current_dir);

  if ( v > 1 )
    v = 1;
  if ( v < -1 )
    v = -1;

  double delta_angle = std::acos(v);
  double sign = current_dir.x * dir.y - current_dir.y * dir.x;
  double current_angle = std::atan2(current_dir.y, current_dir.x);
  double final_angle;

  if ( sign > 0 ) // the item goes on its left
    {
      if ( delta_angle > m_max_angle )
        final_angle = current_angle + m_max_angle;
      else
        final_angle = current_angle + delta_angle;
    }
  else
    {
      if ( delta_angle > m_max_angle )
        final_angle = current_angle - m_max_angle;
      else
        final_angle = current_angle - delta_angle;
    }

  dir.set( std::cos(final_angle), std::sin(final_angle) );

  return final_angle;
} // forced_aiming::compute_direction()
