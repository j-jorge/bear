/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::chain_link class.
 * \author Julien Jorge
 */
#include "universe/link/chain_link.hpp"

#include <limits>

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
bear::universe::chain_link::chain_link
( physical_item& first_item, physical_item& second_item,
  bear::universe::coordinate_type minimal_length,
  bear::universe::coordinate_type maximal_length )
  : base_link(first_item, second_item), m_minimal_length(minimal_length),
    m_maximal_length(maximal_length)
{

} // chain_link::chain_link()

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
bear::universe::chain_link::chain_link
( const reference_point& first_item, const reference_point& second_item,
  universe::coordinate_type minimal_length,
  universe::coordinate_type maximal_length )
  : base_link(first_item, second_item), m_minimal_length(minimal_length),
    m_maximal_length(maximal_length)
{

} // chain_link::chain_link()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the forces applied to each item.
 */
void bear::universe::chain_link::adjust()
{
  force_type dir( m_first_item.get_point(),
                  m_second_item.get_point() );

  const double d = dir.length();
  double delta(0);

  if (d > m_maximal_length)
    delta = d - m_maximal_length;
  else if (d < m_minimal_length)
    delta = d - m_minimal_length; // negative value to move the items apart

  dir.normalize();

  position_type d1(0, 0);
  position_type d2(0, 0);

  const double infinity( std::numeric_limits<double>::infinity() );
  const double m1( m_first_item.get_item().get_mass() );
  const double m2( m_second_item.get_item().get_mass() );

  if ( (m1 != infinity) )
    {
      if ( (m2 != infinity) && (m1 + m2 != 0) )
        {
          d1 = dir * delta * m2 / (m1 + m2);
          d2 = -dir * delta * m1 / (m1 + m2);
        }
      else
        d1 = dir * delta;
    }
  else if ( m2 != infinity )
    d2 = -dir * delta;

  m_first_item.get_item().set_center_of_mass
    ( m_first_item.get_item().get_center_of_mass() + d1 );
  m_second_item.get_item().set_center_of_mass
    ( m_second_item.get_item().get_center_of_mass() + d2 );
} // chain_link::adjust()
