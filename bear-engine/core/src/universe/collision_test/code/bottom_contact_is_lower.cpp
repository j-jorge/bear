/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bottom_contact_is_lower class.
 * \author Julien Jorge
 */
#include "universe/collision_test/bottom_contact_is_lower.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param line We will check if the bottom of the other item was in the part on
 *        the top of this line.
 */
bear::universe::bottom_contact_is_lower::bottom_contact_is_lower
( const line_type& line )
  : m_line(line)
{

} // bottom_contact_is_lower::bottom_contact_is_lower()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param val We will check if the bottom of the other item was lower than this
 *        value.
 */
bear::universe::bottom_contact_is_lower::bottom_contact_is_lower
( bear::universe::coordinate_type val )
  : m_line(0, val, 1, 0)
{

} // bottom_contact_is_lower::bottom_contact_is_lower()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the bottom of the "that" item was
 *        lower than \a m_value, relative to the top of the "self" item.
 * \param info Informations on the collision.
 * \param self The first item in the collision.
 * \param that The other item in the collision.
 */
bool bear::universe::bottom_contact_is_lower::operator()
  ( const collision_info& info, physical_item& self, physical_item& that ) const
{
  bear::universe::position_type origin( that.get_bottom_middle() );
  claw::math::line_2d<bear::universe::coordinate_type> movement
    ( origin, info.other_previous_state().get_bottom_middle() - origin );
  bear::universe::coordinate_type that_bottom;
  bear::universe::coordinate_type y_bound;

  switch ( info.get_collision_side() )
    {
    case zone::middle_right_zone:
      that_bottom = movement.y_value( self.get_right() );
      y_bound = m_line.y_value( self.get_width() );
      break;
    case zone::middle_left_zone:
      that_bottom = movement.y_value( self.get_top_left().x );
      y_bound = m_line.y_value( self.get_top_left().x );
      break;
    case zone::middle_zone:
      that_bottom = info.other_previous_state().get_bottom();
      y_bound = m_line.y_value
        ( info.other_previous_state().get_center_of_mass().x
          - self.get_top_left().x );
      break;
    default:
      {
        CLAW_ASSERT( false, "This test is for left/middle/right collisions" );
      }
    }

  return that_bottom - (int)self.get_top_left().y < y_bound;
} // bottom_contact_is_lower::operator()()
