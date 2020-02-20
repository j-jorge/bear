/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::forced_translation class.
 * \author Sébastien Angibaud
 */
#include "bear/universe/forced_movement/forced_translation.hpp"

#include "bear/universe/physical_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param length The duration of the movement.
 */
bear::universe::forced_translation::forced_translation( time_type length )
  : m_speed(0, 0), m_total_time(length), m_remaining_time(m_total_time),
    m_angle(0), m_force_angle(false)
{

} // forced_translation::forced_translation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param speed The speed of the item.
 * \param length The duration of the movement.
 */
bear::universe::forced_translation::forced_translation
( const speed_type& speed, time_type length )
  : m_speed(speed), m_total_time(length), m_remaining_time(m_total_time),
    m_angle(0), m_force_angle(false)
{

} // forced_translation::forced_translation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this movement.
 */
bear::universe::base_forced_movement*
bear::universe::forced_translation::clone() const
{
  return new forced_translation(*this);
} // forced_translation::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the angle applied to the item.
 * \param angle The angle.
 */
void bear::universe::forced_translation::set_angle( double angle )
{
  m_angle = angle;
} // forced_translation::set_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we force the angle of the item (otherwise the angles are
 *        added).
 * \param bool f Force the angle.
 */
void bear::universe::forced_translation::set_force_angle( bool f )
{
  m_force_angle = f;
} // forced_translation::set_force_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the speed of the item.
 * \param speed The speed.
 */
void bear::universe::forced_translation::set_speed( const speed_type& speed )
{
  m_speed = speed;
} // forced_translation::set_speed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the duration of the movement.
 * \param length The duration of the movement.
 */
void bear::universe::forced_translation::set_total_time( time_type length )
{
  m_total_time = length;
} // forced_translation::set_total_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the speed of the movement.
 */
const bear::universe::speed_type&
bear::universe::forced_translation::get_speed() const
{
  return m_speed;
} // forced_translation::get_speed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the movement is finished.
 */
bool bear::universe::forced_translation::is_finished() const
{
  return m_remaining_time == 0;
} // forced_translation::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the movement.
 */
void bear::universe::forced_translation::do_init()
{
  m_remaining_time = m_total_time;
} // forced_translation::do_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do an iteration of the movement.
 * \param elapsed_time Elapsed time since the last call.
 * \return The remaining time if the movement is finished.
 */
bear::universe::time_type
bear::universe::forced_translation::do_next_position( time_type elapsed_time )
{
  time_type remaining_time(0);

  if ( elapsed_time > m_remaining_time )
    {
      remaining_time = elapsed_time - m_remaining_time;
      elapsed_time = m_remaining_time;
    }

  set_moving_item_position
    ( get_moving_item_position() + m_speed * elapsed_time );

  if ( m_force_angle )
    set_moving_item_system_angle( m_angle );
  else
    set_moving_item_system_angle( get_moving_item_system_angle() + m_angle );

  m_remaining_time -= elapsed_time;

  return remaining_time;
} // forced_translation::do_next_position()
