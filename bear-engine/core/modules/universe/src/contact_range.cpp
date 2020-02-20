/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::contact_range class.
 * \author Julien Jorge
 */
#include "bear/universe/contact_range.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param min The coordinate where the contact starts.
 * \param max The coordinate where the contact ends.
 */
bear::universe::contact_range::contact_range
( coordinate_type min, coordinate_type max )
  : m_min(min), m_max(max)
{

} // contact_range::contact_range()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the coordinate where the contact starts.
 */
bear::universe::coordinate_type bear::universe::contact_range::get_min() const
{
  return m_min;
} // contact_range::get_min()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the coordinate where the contact ends.
 */
bear::universe::coordinate_type bear::universe::contact_range::get_max() const
{
  return m_max;
} // contact_range::get_max()

/*----------------------------------------------------------------------------*/
/**
 * \brief Ensure that empty() returns true.
 */
void bear::universe::contact_range::clear()
{
  m_min = m_max = 0;
} // contact_range::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the end of the contact is not after the beginning.
 */
bool bear::universe::contact_range::empty() const
{
  return m_max <= m_min;
} // contact_range::empty()
