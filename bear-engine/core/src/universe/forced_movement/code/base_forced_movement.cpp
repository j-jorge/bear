/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the base_forced_movement class.
 * \author Julien Jorge.
 */
#include "universe/forced_movement/base_forced_movement.hpp"

#include "universe/forced_movement/center_of_mass_reference_point.hpp"
#include "universe/forced_movement/ratio_reference_point.hpp"
#include "universe/physical_item.hpp"

#include <claw/assert.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::base_forced_movement::base_forced_movement()
  : m_moving_item(NULL), m_auto_remove(false), m_auto_angle(false),
    m_moving_item_ratio(0.5, 0.5), m_moving_item_gap(0, 0)
{

} // base_forced_movement::base_forced_movement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::universe::base_forced_movement::~base_forced_movement()
{
  // nothing to do.
} // base_forced_movement::~base_forced_movement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item.
 */
void bear::universe::base_forced_movement::init()
{
  if ( m_moving_item != NULL )
    do_init();
  else
    claw::logger << claw::log_warning
                 << "base_forced_movement::init(): no item." << std::endl;
} // base_forced_movement::init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop applying this movement to the item.
 */
void bear::universe::base_forced_movement::clear_item()
{
  m_moving_item = NULL;
} // base_forced_movement::clear_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the item concerned by this movement.
 * \param item The item to which is applied this movement.
 */
void bear::universe::base_forced_movement::set_item( physical_item& item )
{
  m_moving_item = &item;
} // base_forced_movement::set_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the reference point used by this movement is valid.
 */
bool bear::universe::base_forced_movement::has_reference_point() const
{
  return m_reference_point.is_valid();
} // base_forced_movement::has_reference_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the reference point used by this movement.
 * \param r The reference point.
 */
void bear::universe::base_forced_movement::set_reference_point
( const reference_point& r )
{
  m_reference_point = r;
} // base_forced_movement::set_reference_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the point used as the reference for the movement.
 */
bear::universe::position_type
bear::universe::base_forced_movement::get_reference_position() const
{
  CLAW_PRECOND( has_reference_item() );
  return m_reference_point.get_point();
} // base_forced_movement::get_reference_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the reference point on the center of mass of a given item.
 * \param item The item of which we use the center of mass.
 */
void bear::universe::base_forced_movement::set_reference_point_on_center
( physical_item& item )
{
  set_reference_point( center_of_mass_reference_point(item) );
} // base_forced_movement::set_reference_point_on_center()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the reference point on a point of a given item calculated with
 *        ratio of size and gap.
 * \param item The item of which we use the center of mass.
 * \param ratio The ratio to applied on size.
 * \param gap The gap to applied.
 */
void bear::universe::base_forced_movement::set_ratio_reference_point
( physical_item& item, const position_type& ratio, const position_type & gap )
{
  set_reference_point( ratio_reference_point(item, ratio, gap) );
} // base_forced_movement::set_ratio_reference_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this movement has a reference item.
 */
bool bear::universe::base_forced_movement::has_reference_item() const
{
  return m_reference_point.has_item();
} // base_forced_movement::has_reference_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the item used as a reference in the movement.
 * \pre has_reference_item()
 */
bear::universe::physical_item&
bear::universe::base_forced_movement::get_reference_item() const
{
  CLAW_PRECOND( has_reference_item() );

  return m_reference_point.get_item();
} // base_forced_movement::get_reference_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the movement removes himself from the moving item when
 *        finished.
 */
void bear::universe::base_forced_movement::set_auto_remove(bool b)
{
  m_auto_remove = b;
} // base_forced_movement::set_auto_remove()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the angle of the moving item is updated according to its
 *        angular speed.
 */
void bear::universe::base_forced_movement::set_auto_angle(bool b)
{
  m_auto_angle = b;
} // base_forced_movement::set_auto_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the ratio of the size to use when computing moving item's
 *        position.
 */
bear::universe::position_type
bear::universe::base_forced_movement::get_moving_item_ratio() const
{
  return m_moving_item_ratio;
} // base_forced_movement::get_moving_item_ratio()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the ratio of the size to use when computing moving item's
 *        position.
 * \param ratio The ratio to use.
 */
void bear::universe::base_forced_movement::set_moving_item_ratio
(const position_type & ratio)
{
  m_moving_item_ratio = ratio;
} // base_forced_movement::set_moving_item_ratio()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the gap to use when computing moving item's position.
 */
bear::universe::position_type
bear::universe::base_forced_movement::get_moving_item_gap() const
{
  return m_moving_item_gap;
} // base_forced_movement::get_moving_item_gap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the gap to use when computing moving item's position.
 * \param gap The ratio to use.
 */
void bear::universe::base_forced_movement::set_moving_item_gap
(const position_type & gap)
{
  m_moving_item_gap = gap;
} // base_forced_movement::set_moving_item_gap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the item to the next position.
 * \param elapsed_time Elapsed time since the last call.
 * \return The remaining time if the movement is finished.
 */
bear::universe::time_type
bear::universe::base_forced_movement::next_position( time_type elapsed_time )
{
  time_type remaining_time(elapsed_time);

  if ( m_moving_item != NULL )
    {
      const position_type pos( get_moving_item_position() );
      const double angle( m_moving_item->get_system_angle() );

      remaining_time = do_next_position(elapsed_time);

      const time_type dt( elapsed_time - remaining_time );

      if ( m_auto_angle )
        update_angle( dt );

      if ( dt > 0 )
        adjust_cinetic( pos, angle, dt );

      if (is_finished() && m_auto_remove)
        m_moving_item->clear_forced_movement();
    }
  else
    claw::logger << claw::log_warning
                 << "base_forced_movement::next_position(): no item."
                 << std::endl;

  return remaining_time;
} // base_forced_movement::next_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the item concerned by this movement.
 */
bear::universe::physical_item& bear::universe::base_forced_movement::get_item()
{
  CLAW_PRECOND( m_moving_item != NULL );

  return *m_moving_item;
} // base_forced_movement::get_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the item concerned by this movement.
 */
const bear::universe::physical_item&
bear::universe::base_forced_movement::get_item() const
{
  CLAW_PRECOND( m_moving_item != NULL );

  return *m_moving_item;
} // base_forced_movement::get_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of moving item.
 * \param pos The position to give at moving item.
 */
void bear::universe::base_forced_movement::set_moving_item_position
( const position_type& pos )
{
  if ( m_moving_item != NULL )
    {
      m_moving_item->set_bottom_left
        ( pos.x - m_moving_item->get_width() *
          m_moving_item_ratio.x - m_moving_item_gap.x,
          pos.y - m_moving_item->get_height() *
          m_moving_item_ratio.y - m_moving_item_gap.y );
    }
} // base_forced_movement::set_moving_item_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of moving item.
 */
const bear::universe::position_type
bear::universe::base_forced_movement::get_moving_item_position() const
{
  position_type pos(0, 0);

  if ( m_moving_item != NULL )
    {
      pos.x = m_moving_item->get_left() + m_moving_item->get_width() *
        m_moving_item_ratio.x + m_moving_item_gap.x;
      pos.y = m_moving_item->get_bottom() + m_moving_item->get_height() *
        m_moving_item_ratio.y + m_moving_item_gap.y;
    }

  return pos;
} //base_forced_movement::get_moving_item_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the system angle of moving item.
 */
void bear::universe::base_forced_movement::set_moving_item_system_angle
( double angle )
{
  if ( m_moving_item != NULL )
    m_moving_item->set_system_angle(angle);
} // base_forced_movement::set_moving_item_system_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the system angle of moving item.
 */
double
bear::universe::base_forced_movement::get_moving_item_system_angle() const
{
  double angle(0);

  if ( m_moving_item != NULL )
    angle = m_moving_item->get_system_angle();

  return angle;
} // base_forced_movement::set_moving_item_system_angle()

/*----------------------------------------------------------------------------*/
/**
 * Update the system angle of the moving item.
 * \param elapsed_time The time during which the item moved.
 */
void bear::universe::base_forced_movement::update_angle
( time_type elapsed_time ) const
{
  const double angular_speed( m_moving_item->get_angular_speed() );
  m_moving_item->set_system_angle
    ( m_moving_item->get_system_angle() + angular_speed * elapsed_time );
} // base_forced_movement::update_angle()

/*----------------------------------------------------------------------------*/
/**
 * Adjust the informations about the cinetic for the moving item.
 * \param initial_position The initial position of the moving item.
 * \param initial_angle The initial angle of the moving item.
 * \param dt The time during which the item moved.
 */
void bear::universe::base_forced_movement::adjust_cinetic
( const position_type& initial_position, double initial_angle,
  time_type dt ) const
{
  m_moving_item->set_angular_speed
    ( (m_moving_item->get_system_angle() - initial_angle) / dt );
  m_moving_item->set_speed
    ( (get_moving_item_position() - initial_position) / dt );
} // base_forced_movement::adjust_cinetic()
