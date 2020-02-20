/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::block class.
 * \author Sebastien Angibaud
 */
#include "bear/generic_items/block.hpp"

#include "bear/universe/collision_align_policy.hpp"
#include "bear/universe/collision_info.hpp"

#include "bear/engine/item_brick/loader/activable_sides_loader.hpp"

BASE_ITEM_EXPORT( block, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::block::block()
: m_collision_threshold(10)
{
  set_weak_collisions(false);
} // block::block()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::block::build()
{
  super::build();

  init_default_contact_mode
    ( top_side_is_active(), bottom_side_is_active(), left_side_is_active(),
      right_side_is_active() );
} // block::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c unsigned \c integer
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::block::set_u_integer_field( const std::string& name, unsigned int value )
{
  bool result(true);

  if ( name == "block.collision_threshold" )
    m_collision_threshold = value;
  else
    result = super::set_u_integer_field(name, value);

  return result;
} // block::set_u_integer_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::block::populate_loader_map( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( engine::activable_sides_loader(*this) );
} // block::populate_loader_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the collision is on a solid side and align the other item.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 * \return true if \a that has been aligned.
 */
bool bear::block::collision_check_and_align
( engine::base_item& that, universe::collision_info& info )
{
  if ( !satisfy_collision_condition(that) )
    return false;

  bool align(false);

  switch( choose_alignment_side(that, info) )
    {
    case universe::zone::bottom_zone:
      align = align_bottom(that, info);
      break;
    case universe::zone::top_zone:
      align = align_top(that, info);
      break;
    case universe::zone::middle_left_zone:
      align = align_left(that, info);
      break;
    case universe::zone::middle_right_zone:
      align = align_right(that, info);
      break;
    case universe::zone::middle_zone:
      align = align_middle(that, info);
      break;
    default: { CLAW_FAIL( "Invalid collision side." ); }
    }

  return align;
} // block::collision_check_and_align()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call collision_check_and_align().
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::block::collision
( engine::base_item& that, universe::collision_info& info )
{
  collision_check_and_align(that, info);
} // block::collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Choose the size on which the other item should be aligned.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bear::universe::zone::position
bear::block::choose_alignment_side
( const engine::base_item& that, const universe::collision_info& info ) const
{
  universe::zone::position result( info.get_collision_side() );

  switch( info.get_collision_side() )
    {
    case universe::zone::bottom_zone:
    case universe::zone::top_zone:
      {
        if ( (info.other_previous_state().get_left() >= get_right())
             && (that.get_left() >= get_right() - m_collision_threshold) )
          result = universe::zone::middle_right_zone;
        else if ( (info.other_previous_state().get_right() <= get_left())
                  && (that.get_right() <= get_left() + m_collision_threshold ) )
          result = universe::zone::middle_left_zone;
      }
      break;
    case universe::zone::middle_left_zone:
    case universe::zone::middle_right_zone:
      {
        if ( (info.other_previous_state().get_bottom() >= get_top())
             && (that.get_bottom() >= get_top() - m_collision_threshold) )
          result = universe::zone::top_zone;
        else if ( (info.other_previous_state().get_top() <= get_bottom())
                  && (that.get_top() <= get_bottom() + m_collision_threshold ) )
          result = universe::zone::bottom_zone;
      }
      break;
    case universe::zone::middle_zone:
      break;
    default: { CLAW_FAIL( "Invalid collision side." ); }
    }

  return result;
} // block::choose_alignment_side()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align a colliding item on the bottom side.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::block::align_bottom
( engine::base_item& that, universe::collision_info& info )
{
  const universe::collision_align_policy policy
    ( get_bottom_contact_mode(info) );
  const bool align =
    bottom_side_is_active() && collision_align_bottom(info, policy);

  if ( align )
    post_alignment( that, get_bottom_friction() );

  return align;
} // block::align_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align a colliding item on the top side.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::block::align_top
( engine::base_item& that, universe::collision_info& info )
{
  const universe::collision_align_policy policy
    ( get_top_contact_mode(info) );
  const bool align = top_side_is_active() && collision_align_top(info, policy);

  if ( align )
    post_alignment( that, get_top_friction() );
  
  return align;
} // block::align_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align a colliding item on the left side.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::block::align_left
( engine::base_item& that, universe::collision_info& info )
{
  const universe::collision_align_policy policy
    ( get_left_contact_mode(info) );
  const bool align =
    left_side_is_active() && collision_align_left(info, policy);

  if ( align )
    post_alignment( that, get_left_friction() );

  return align;
} // block::align_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align a colliding item on the right side.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::block::align_right
( engine::base_item& that, universe::collision_info& info )
{
  const universe::collision_align_policy policy
    ( get_right_contact_mode(info) );
  const bool align =
    right_side_is_active() && collision_align_right(info, policy);

  if ( align )
    post_alignment( that, get_right_friction() );

  return align;
} // block::align_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align a colliding item on the middle of the item.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::block::align_middle
( engine::base_item& that, universe::collision_info& info )
{
  const universe::collision_align_policy policy
    ( get_middle_contact_mode(info) );
  const bool align = collision_middle(info, policy);

  if ( align )
    post_alignment( that, that.get_contact_friction() );

  return align;
} // block::align_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the common stuff to do after aligning an item.
 * \param that The other item of the collision.
 * \param f The contact friction to apply to \a that.
 */
void bear::block::post_alignment( engine::base_item& that, double f )
{
  that.set_contact_friction(f);
  that.set_contact_angle(0);
  do_z_shift(that);
} // block::post_alignment()
