/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the forced_movement class.
 * \author Julien Jorge.
 */
#include "universe/forced_movement/forced_movement.hpp"

#include "universe/forced_movement/base_forced_movement.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::forced_movement::forced_movement()
  : m_movement(NULL)
{

} // forced_movement::forced_movement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bear::universe::forced_movement::forced_movement( const forced_movement& that )
{
  if ( that.is_null() )
    m_movement = NULL;
  else
    m_movement = that.m_movement->clone();
} // forced_movement::forced_movement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param m The effective movement.
 */
bear::universe::forced_movement::forced_movement
( const base_forced_movement& m )
  : m_movement( m.clone() )
{

} // forced_movement::forced_movement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::universe::forced_movement::~forced_movement()
{
  clear();
} // forced_movement::~forced_movement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment.
 * \param that The instance to copy from.
 */
bear::universe::forced_movement&
bear::universe::forced_movement::operator=( forced_movement that )
{
  std::swap(m_movement, that.m_movement);
  return *this;
} // forced_movement::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the forced movement contain an effective movement.
 */
bool bear::universe::forced_movement::is_null() const
{
  return m_movement == NULL;
} // forced_movement::is_null()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the effective movement.
 */
void bear::universe::forced_movement::clear()
{
  delete m_movement;
  m_movement = NULL;
} // forced_movement::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item.
 */
void bear::universe::forced_movement::init()
{
  CLAW_PRECOND( !is_null() );
  m_movement->init();
} // forced_movement::init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop applying this movement to the item.
 */
void bear::universe::forced_movement::clear_item()
{
  CLAW_PRECOND( !is_null() );
  m_movement->clear_item();
} // forced_movement::clear_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the item concerned by this movement.
 * \param item The item to which is applied this movement.
 */
void bear::universe::forced_movement::set_item( physical_item& item )
{
  CLAW_PRECOND( !is_null() );
  m_movement->set_item(item);
} // forced_movement::set_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the reference point used by this movement is valid.
 */
bool bear::universe::forced_movement::has_reference_point() const
{
  CLAW_PRECOND( !is_null() );
  return m_movement->has_reference_point();
} // forced_movement::has_reference_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the reference point used by this movement.
 * \param r The reference point.
 */
void bear::universe::forced_movement::set_reference_point
( const reference_point& r )
{
  CLAW_PRECOND( !is_null() );
  m_movement->set_reference_point(r);
} // forced_movement::set_reference_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the point used as the reference for the movement.
 */
bear::universe::position_type
bear::universe::forced_movement::get_reference_position() const
{
  CLAW_PRECOND( !is_null() );
  return m_movement->get_reference_position();
} // forced_movement::get_reference_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the item used as a reference by this movement.
 * \param item The item used as a reference by this movement.
 */
void bear::universe::forced_movement::set_reference_point_on_center
( physical_item& item )
{
  CLAW_PRECOND( !is_null() );
  m_movement->set_reference_point_on_center( item );
} // forced_movement::set_reference_point_on_center()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this movement has a reference item.
 */
bool bear::universe::forced_movement::has_reference_item() const
{
  CLAW_PRECOND( !is_null() );
  return m_movement->has_reference_item();
} // forced_movement::has_reference_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the item used as a reference in the movement.
 * \pre has_reference_item()
 */
bear::universe::physical_item&
bear::universe::forced_movement::get_reference_item() const
{
  CLAW_PRECOND( !is_null() );
  return m_movement->get_reference_item();
} // forced_movement::get_reference_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the movement removes himself from the moving item when
 *        finished.
 */
void bear::universe::forced_movement::set_auto_remove(bool b)
{
  CLAW_PRECOND( !is_null() );
  m_movement->set_auto_remove(b);
} // forced_movement::set_auto_remove()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the angle of the moving item is updated according to its
 *        angular speed.
 */
void bear::universe::forced_movement::set_auto_angle(bool b)
{
  CLAW_PRECOND( !is_null() );
  m_movement->set_auto_angle(b);
} // forced_movement::set_auto_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the ratio of the size to use when computing moving item's
 *        position.
 */
bear::universe::position_type
bear::universe::forced_movement::get_moving_item_ratio() const
{
  CLAW_PRECOND( !is_null() );
  return m_movement->get_moving_item_ratio();
} // forced_movement::get_moving_item_ratio()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set ratio of size to compute moving item's position.
 * \param ratio The ratio to use.
 */
void bear::universe::forced_movement::set_moving_item_ratio
(const position_type & ratio)
{
  CLAW_PRECOND( !is_null() );
  m_movement->set_moving_item_ratio(ratio);
} // forced_movement::set_moving_item_ratio()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the gap to use when computing moving item's position.
 */
bear::universe::position_type
bear::universe::forced_movement::get_moving_item_gap() const
{
  CLAW_PRECOND( !is_null() );
  return m_movement->get_moving_item_gap();
} // forced_movement::get_moving_item_gap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set gap to compute moving item's position.
 * \param gap The ratio to use.
 */
void bear::universe::forced_movement::set_moving_item_gap
(const position_type & gap)
{
  CLAW_PRECOND( !is_null() );
  m_movement->set_moving_item_gap(gap);
} // forced_movement::set_moving_item_gap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the item to the next position.
 * \param elapsed_time Elapsed time since the last call.
 * \return The remaining time if the movement is finished.
 */
bear::universe::time_type
bear::universe::forced_movement::next_position( time_type elapsed_time )
{
  CLAW_PRECOND( !is_null() );
  return m_movement->next_position(elapsed_time);
} // forced_movement::next_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the movement is finished.
 */
bool bear::universe::forced_movement::is_finished() const
{
  CLAW_PRECOND( !is_null() );
  return m_movement->is_finished();
} // forced_movement::is_finished()
