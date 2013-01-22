/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the center_of_mass_reference_point class.
 * \author Julien Jorge
 */
#include "universe/forced_movement/center_of_mass_reference_point.hpp"

#include "universe/physical_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item of which the center of mass is returned.
 */
bear::universe::center_of_mass_reference_point::center_of_mass_reference_point
( physical_item& item )
  : m_item(item)
{

} // center_of_mass_reference_point::center_of_mass_reference_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Duplicate this instance.
 */
bear::universe::base_reference_point*
bear::universe::center_of_mass_reference_point::clone() const
{
  return new center_of_mass_reference_point(*this);
} // center_of_mass_reference_point::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this reference is usable.
 */
bool bear::universe::center_of_mass_reference_point::is_valid() const
{
  return has_item();
} // center_of_mass_reference_point::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the center of mass of the item.
 */
bear::universe::position_type
bear::universe::center_of_mass_reference_point::get_point() const
{
  CLAW_PRECOND( has_item() );
  return m_item->get_center_of_mass();
} // center_of_mass_reference_point::get_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is still valid.
 */
bool bear::universe::center_of_mass_reference_point::has_item() const
{
  return m_item != NULL;
} // center_of_mass_reference_point::has_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the item of which the center of mass is returned.
 */
bear::universe::physical_item&
bear::universe::center_of_mass_reference_point::get_item() const
{
  CLAW_PRECOND( has_item() );
  return *m_item;
} // center_of_mass_reference_point::get_item()
