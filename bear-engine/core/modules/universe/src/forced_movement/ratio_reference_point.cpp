/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the ratio_reference_point class.
 * \author Sebastien Angibaud
 */
#include "bear/universe/forced_movement/ratio_reference_point.hpp"

#include "bear/universe/physical_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item of which the center of mass is returned.
 */
bear::universe::ratio_reference_point::ratio_reference_point
( physical_item& item, const universe::position_type& ratio,
  const universe::position_type& gap )
  : m_item(item), m_ratio(ratio), m_gap(gap)
{

} // ratio_reference_point::ratio_reference_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Duplicate this instance.
 */
bear::universe::base_reference_point*
bear::universe::ratio_reference_point::clone() const
{
  return new ratio_reference_point(*this);
} // ratio_reference_point::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this reference is usable.
 */
bool bear::universe::ratio_reference_point::is_valid() const
{
  return has_item();
} // ratio_reference_point::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the center of mass of the item.
 */
bear::universe::position_type
bear::universe::ratio_reference_point::get_point() const
{
  CLAW_PRECOND( has_item() );

  return bear::universe::position_type
    ( m_item->get_left() + m_item->get_width() * m_ratio.x + m_gap.x,
      m_item->get_bottom() + m_item->get_height() * m_ratio.y + m_gap.y );
} // ratio_reference_point::get_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is still valid.
 */
bool bear::universe::ratio_reference_point::has_item() const
{
  return m_item != NULL;
} // ratio_reference_point::has_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the item of which the center of mass is returned.
 */
bear::universe::physical_item&
bear::universe::ratio_reference_point::get_item() const
{
  CLAW_PRECOND( has_item() );
  return *m_item;
} // ratio_reference_point::get_item()
