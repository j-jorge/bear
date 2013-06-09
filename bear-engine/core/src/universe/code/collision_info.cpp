/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
#include "universe/collision_info.hpp"

#include "universe/alignment/align_bottom.hpp"
#include "universe/alignment/align_bottom_left.hpp"
#include "universe/alignment/align_bottom_right.hpp"
#include "universe/alignment/align_left.hpp"
#include "universe/alignment/align_right.hpp"
#include "universe/alignment/align_top.hpp"
#include "universe/alignment/align_top_left.hpp"
#include "universe/alignment/align_top_right.hpp"
#include "universe/collision_repair.hpp"
#include "universe/physical_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param previous_self Previous state of the reference item.
 * \param previous_that Previous state of the other item.
 * \param self The reference item.
 * \param that The other item.
 * \param repair Informations for repairing the collision.
 */
bear::universe::collision_info::collision_info
( const physical_item_state& previous_self,
  const physical_item_state& previous_that,
  physical_item& self, physical_item& that, collision_repair& repair )
  : m_previous_self( previous_self ), m_previous_other( previous_that ),
    m_other(that), m_repair(repair)
{
  alignment* align = find_alignment();

  apply_alignment( *align, self );

  delete align;
} // collision_info::collision_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the side of the reference item hit first by the other item.
 */
bear::universe::zone::position
bear::universe::collision_info::get_collision_side() const
{
  return m_side;
} // collision_info::get_collision_side()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the other item when it hit the reference item.
 */
const bear::universe::position_type&
bear::universe::collision_info::get_bottom_left_on_contact() const
{
  return m_position_on_contact;
} // collision_info::get_bottom_left_on_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the previous state of the other item.
 */
const bear::universe::physical_item_state&
bear::universe::collision_info::other_previous_state() const
{
  return m_previous_other;
} // collision_info::other_previous_state()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the previous state of the reference item.
 */
const bear::universe::physical_item_state&
bear::universe::collision_info::reference_previous_state() const
{
  return m_previous_self;
} // collision_info::reference_previous_state()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the other item.
 */
bear::universe::physical_item&
bear::universe::collision_info::other_item() const
{
  return m_other;
} // collision_info::other_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the informations for repairing the collision.
 */
bear::universe::collision_repair&
bear::universe::collision_info::get_collision_repair() const
{
  return m_repair;
} // collision_info::get_collision_repair()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the correct alignment class to compute the position that the
 *        other item had when the collision occured.
 */
bear::universe::alignment*
bear::universe::collision_info::find_alignment() const
{
  rectangle_type self_previous_box;
  rectangle_type that_previous_box;

  self_previous_box = m_previous_self.get_bounding_box();
  that_previous_box = m_previous_other.get_bounding_box();

  zone::position z = zone::find(that_previous_box, self_previous_box );
  alignment* result(NULL);

  switch( z )
    {
    case zone::top_left_zone:     result = new align_top_left(); break;
    case zone::top_zone:          result = new align_top(); break;
    case zone::top_right_zone:    result = new align_top_right(); break;
    case zone::middle_left_zone:  result = new align_left(); break;
    case zone::middle_zone:       result = new alignment(); break;
    case zone::middle_right_zone: result = new align_right(); break;
    case zone::bottom_left_zone:  result = new align_bottom_left(); break;
    case zone::bottom_zone:       result = new align_bottom(); break;
    case zone::bottom_right_zone: result = new align_bottom_right(); break;
    default:
      {
        CLAW_FAIL( "Invalid side." );
      }
    }

  return result;
} // collision_info::find_alignment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the position that the other item had when the collision occured.
 * \param align The alignment to use to compute the position.
 * \param self The reference item.
 */
void bear::universe::collision_info::apply_alignment
( const alignment& align, const physical_item& self )
{
  rectangle_type self_new_box;
  rectangle_type that_new_box;

  self_new_box = self.get_bounding_box();
  that_new_box = m_other.get_bounding_box();

  align.align( self_new_box, m_previous_other.get_bottom_left(), that_new_box );

  m_position_on_contact = that_new_box.bottom_left();
  m_side = zone::find(that_new_box, self_new_box);

  // we want m_side to be a side.
  switch( m_side )
    {
    case zone::top_right_zone:
    case zone::top_left_zone:
      m_side = zone::top_zone; break;
    case zone::bottom_left_zone:
    case zone::bottom_right_zone:
      m_side = zone::bottom_zone; break;
    default:
      { }
    }
} // collision_info::apply_alignment()
