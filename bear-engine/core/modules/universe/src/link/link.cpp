/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::link class.
 * \author Julien Jorge
 */
#include "bear/universe/link/link.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param first_item The first linked item.
 * \param second_item The second linked item.
 * \param strength The strength of the elastic.
 * \param minimal_length The length under which the link moves the items apart
 *        from each other.
 * \param maximal_length The length over which the link moves the items toward
 *        each other.
 */
bear::universe::link::link
( physical_item& first_item, physical_item& second_item,
  double strength, bear::universe::coordinate_type minimal_length,
  bear::universe::coordinate_type maximal_length )
  : base_link(first_item, second_item), m_strength(strength),
    m_minimal_length(minimal_length), m_maximal_length(maximal_length)
{

} // link::link()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param first_item The first linked item.
 * \param second_item The second linked item.
 * \param minimal_length The length under which the link moves the items apart
 *        from each other.
 * \param maximal_length The length over which the link moves the items toward
 *        each other.
 */
bear::universe::link::link
( const reference_point& first_item, const reference_point& second_item,
  double strength, universe::coordinate_type minimal_length,
  universe::coordinate_type maximal_length )
  : base_link(first_item, second_item), m_strength(strength),
    m_minimal_length(minimal_length), m_maximal_length(maximal_length)
{

} // link::link()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the forces applied to each item.
 */
void bear::universe::link::adjust()
{
  force_type dir( m_second_item.get_point(),
                  m_first_item.get_point() );

  const double d = dir.length();
  double delta(0);

  if (d > m_maximal_length)
    delta = d - m_maximal_length;
  else if (d < m_minimal_length)
    delta = d - m_minimal_length; // negative value to move the items apart

  dir.normalize();
  dir *= m_strength * delta / d;

  m_first_item.get_item().add_external_force(-dir);
  m_second_item.get_item().add_external_force(dir);
} // link::adjust()
