/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::forced_stay_around class.
 * \author Julien Jorge
 */
#include "bear/universe/forced_movement/forced_stay_around.hpp"

#include "bear/universe/physical_item.hpp"

#include <cmath>
#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param length The duration of the movement.
 */
bear::universe::forced_stay_around::forced_stay_around( time_type length )
  : m_max_angle( std::numeric_limits<double>::infinity() ),
    m_speed( std::numeric_limits<double>::infinity() ),
    m_max_distance( std::numeric_limits<coordinate_type>::infinity() ),
    m_total_time( length ), m_remaining_time(m_total_time), m_apply_angle(false)
{

} // forced_stay_around::forced_stay_around()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this movement.
 */
bear::universe::base_forced_movement*
bear::universe::forced_stay_around::clone() const
{
  return new forced_stay_around(*this);
} // forced_stay_around::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the maximum angle when the direction changes.
 * \param a The angle.
 */
void bear::universe::forced_stay_around::set_max_angle( double a )
{
  if (a < 0)
    m_max_angle = -a;
  else
    m_max_angle = a;
} // forced_stay_around::set_max_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the constant speed of the moving item.
 * \param s The speed.
 */
void bear::universe::forced_stay_around::set_speed( double s )
{
  if( s < 0 )
    m_speed = -s;
  else
    m_speed = s;
} // forced_stay_around::set_max_speed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the maximum distance to the center.
 * \param d The distance.
 */
void bear::universe::forced_stay_around::set_max_distance( coordinate_type d )
{
  if( d < 0 )
    m_max_distance = -d;
  else
    m_max_distance = d;
} // forced_stay_around::set_max_distance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we apply the angle of the movement to the item.
 * \param b Apply or not.
 */
void bear::universe::forced_stay_around::set_apply_angle( bool b )
{
  m_apply_angle = b;
} // forced_stay_around::set_apply_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the duration of the movement.
 * \param length The duration of the movement.
 */
void bear::universe::forced_stay_around::set_total_time( time_type length )
{
  m_total_time = length;
} // forced_stay_around::set_total_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the movement is finished.
 */
bool bear::universe::forced_stay_around::is_finished() const
{
  return !has_reference_point() || (m_remaining_time == 0);
} // forced_stay_around::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the movement.
 */
void bear::universe::forced_stay_around::do_init()
{
  m_remaining_time = m_total_time;

  if ( has_reference_point() )
    {
      m_previous_reference_position = get_reference_position();
      m_position_delta = get_moving_item_position() - get_reference_position();
    }
  else
    {
      m_position_delta = position_type(0, 0);
      m_previous_reference_position = position_type(0, 0);
    }
} // forced_stay_around::do_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do an iteration of the movement.
 * \param elapsed_time Elapsed time since the last call.
 * \return The remaining time if the movement is finished.
 */
bear::universe::time_type
bear::universe::forced_stay_around::do_next_position( time_type elapsed_time )
{
  time_type remaining_time(0);

  if ( has_reference_point() )
    {
      remaining_time = compute_remaining_time(elapsed_time);

      // The direction of the moving item relatively to the reference is the
      // movement done during the elapsed time minus the movement of the
      // reference.
      const vector_type current_dir
        ( get_item().get_speed()
          - ( get_reference_position() - m_previous_reference_position )
          / elapsed_time );
      vector_type dir;

      double a = compute_direction(dir, current_dir);

      m_position_delta += dir * m_speed * elapsed_time;

      set_moving_item_position( get_reference_position() + m_position_delta );

      if ( m_apply_angle )
        set_moving_item_system_angle( a );

      m_previous_reference_position = get_reference_position();
    }

  return remaining_time;
} // forced_stay_around::do_next_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the remaining time after this step of the movement.
 * \param elapsed_time Elapsed time since the last call to do_next_position().
 */
bear::universe::time_type
bear::universe::forced_stay_around::compute_remaining_time
( time_type& elapsed_time )
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
} // forced_stay_around::compute_remaining_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the direction to apply to the item.
 * \param dir (out) The direction.
 * \param current_dir The relative direction of the moving item according to the
 *        reference.
 * \return The angle of the direction.
 */
double
bear::universe::forced_stay_around::compute_direction
( vector_type& dir, vector_type current_dir ) const
{
  // try to come back to the reference
  dir = vector_type( get_moving_item_position(), get_reference_position() );

  // random angle with a preference toward the reference if we are far from it
  const coordinate_type dist_from_ref
    ( std::min(1.0, dir.length() / m_max_distance) );
  const double max_a(2 * 3.14159 * (1 - dist_from_ref));
  const double a = max_a * (double)rand() / RAND_MAX - max_a / 2;

  current_dir.normalize();
  dir.normalize();

  double v = dir.dot_product(current_dir);

  if ( v > 1 )
    v = 1;
  if ( v < -1 )
    v = -1;

  double delta_angle = std::acos(v) + a;
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
} // forced_stay_around::compute_direction()
