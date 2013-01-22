/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::descending_ceiling class.
 * \author Julien Jorge
 */
#include "generic_items/descending_ceiling.hpp"

#include "universe/collision_info.hpp"
#include "universe/collision_repair.hpp"
#include "universe/world.hpp"
#include "engine/export.hpp"

BASE_ITEM_EXPORT( descending_ceiling, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::descending_ceiling::descending_ceiling()
  : m_opposite_side_is_active(false), m_left_side_is_active(false),
    m_right_side_is_active(false), m_apply_angle(true), m_line(0, 0, 0, 0)
{

} // descending_ceiling::descending_ceiling()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c <real>.
 * \param name The name of the field to set.
 * \param value The new value of the field.
 */
bool bear::descending_ceiling::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "descending_ceiling.steepness" )
    m_line.direction.y = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // descending_ceiling::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field to set.
 * \param value The new value of the field.
 */
bool
bear::descending_ceiling::set_bool_field( const std::string& name, bool value )
{
  bool result = true;

  if ( name == "descending_ceiling.opposite_side_is_active" )
    m_opposite_side_is_active = value;
  else if ( name == "descending_ceiling.left_side_is_active" )
    m_left_side_is_active = value;
  else if ( name == "descending_ceiling.right_side_is_active" )
    m_right_side_is_active = value;
  else if ( name == "descending_ceiling.apply_angle" )
    m_apply_angle = value;
  else
    result = super::set_bool_field(name, value);

  return result;
} // descending_ceiling::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is correctly initialized.
 */
bool bear::descending_ceiling::is_valid() const
{
  return (m_line.direction.y != 0) && super::is_valid();
} // descending_ceiling::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::descending_ceiling::build()
{
  super::build();

  m_line.origin.set(get_left(), get_bottom());
  m_line.direction.x = get_width();

  if ( m_line.direction.y < 0 )
    m_line.origin.y -= m_line.direction.y;

  init_default_contact_mode
    ( m_opposite_side_is_active, false, m_left_side_is_active,
      m_right_side_is_active );
} // descending_ceiling::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the steepness of the descending_ceiling.
 */
bear::universe::coordinate_type bear::descending_ceiling::get_steepness() const
{
  return m_line.direction.y;
} // descending_ceiling::get_steepness()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::descending_ceiling::collision_as_ceiling
( engine::base_item& that, universe::collision_info& info )
{
  bool align_as_block(false);
  bool align_as_ceiling(false);

  switch( info.get_collision_side() )
    {
    case universe::zone::top_zone:
      align_as_block = m_opposite_side_is_active;
      break;
    case universe::zone::bottom_zone:
      align_as_ceiling = true;
      break;
    case universe::zone::middle_left_zone:
      if ( check_left_contact_as_ceiling(that, info) )
        align_as_ceiling = true;
      else
        align_as_block = m_left_side_is_active;
      break;
    case universe::zone::middle_right_zone:
      if ( check_right_contact_as_ceiling(that, info) )
        align_as_ceiling = true;
      else
        align_as_block = m_right_side_is_active;
      break;
    case universe::zone::middle_zone:
      align_as_ceiling = check_top_below_ceiling(that, info);
      break;
    default: { CLAW_ASSERT( false, "Invalid collision side." ); }
    }

  if ( satisfy_collision_condition(that) )
    {
      if ( align_as_ceiling )
        align_on_ceiling(that, info);
      else if ( align_as_block )
        default_collision(info);
      else
        align_nearest_edge(that, info);
    }
} // descending_ceiling::collision_as_ceiling()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call collision_as_ceiling().
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::descending_ceiling::collision
( engine::base_item& that, universe::collision_info& info )
{
  collision_as_ceiling(that, info);
} // descending_ceiling::collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if an item colliding on the left is aligned on the ground.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::descending_ceiling::check_left_contact_as_ceiling
( engine::base_item& that, universe::collision_info& info ) const
{
  bool result = false;

  // the ceiling goes from top left to bottom right
  if ( m_line.direction.y < 0 )
    result =
      info.get_bottom_left_on_contact().y + info.other_item().get_height()
      >= m_line.y_value( get_left() );

  return result;
} // descending_ceiling::check_left_contact_as_ceiling()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if an item colliding on the right is aligned on the ground.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::descending_ceiling::check_right_contact_as_ceiling
( engine::base_item& that, universe::collision_info& info ) const
{
  bool result = false;

  // the ceiling goes from bottom left to top right
  if ( m_line.direction.y > 0 )
    result =
      info.get_bottom_left_on_contact().y + info.other_item().get_height()
      >= m_line.y_value( get_right() );

  return result;
} // descending_ceiling::check_right_contact_as_ceiling()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the bottom of the other item was above the ground at
 *        collision time.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::descending_ceiling::check_top_below_ceiling
( engine::base_item& that, universe::collision_info& info ) const
{
  const universe::position_type pos
    (info.other_previous_state().get_top_middle());

  return pos.y <= m_line.y_value(pos.x);
} // descending_ceiling::check_top_below_ceiling()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align \a that on the ceiling line.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::descending_ceiling::align_on_ceiling
( engine::base_item& that, universe::collision_info& info )
{
  bool result = false;

  const universe::coordinate_type pos_x
    ( info.get_bottom_left_on_contact().x + that.get_width() / 2 );

  if ( (pos_x >= get_left()) && (pos_x <= get_right())
       && item_crossed_down_up(that, info) )
    {
      universe::position_type pos
        ( info.get_bottom_left_on_contact().x,
          m_line.y_value(pos_x) - info.other_item().get_height() );

      if ( collision_align_bottom(info, pos) )
        {
          result = true;

          if ( m_apply_angle )
            apply_angle_to(that, info);
        }
    }

  return result;
} // descending_ceiling::align_on_ceiling()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align \a that on the nearest vertical edge.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::descending_ceiling::align_nearest_edge
( engine::base_item& that, universe::collision_info& info )
{
  bool result = false;

  if ( info.reference_previous_state().get_center_of_mass().x
       <= info.other_previous_state().get_center_of_mass().x )
    {
      if (m_right_side_is_active)
        result = collision_align_right(info);
    }
  else if (m_left_side_is_active)
    result = collision_align_left(info);

  return result;
} // descending_ceiling::align_nearest_edge()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the other item has crossed the surface following a top to down
 *        direction.
 * \param that The other item in the collision.
 * \param info Informations on the collision.
 */
bool bear::descending_ceiling::item_crossed_down_up
( engine::base_item& that, const universe::collision_info& info ) const
{
  bool result = false;

  if ( that.get_top() >= m_line.y_value(that.get_center_of_mass().x) )
    {
      const universe::position_type other_prev_top
        ( info.other_previous_state().get_top_middle() );

      if ( other_prev_top.x < get_left() )
        result = other_prev_top.y <= m_line.origin.y;
      else if ( other_prev_top.x > get_right() )
        result = other_prev_top.y <= m_line.y_value( get_right() );
      else
        result = other_prev_top.y <= m_line.y_value(other_prev_top.x);
    }

  return result;
} // descending_ceiling::item_crossed_down_up()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the angle of the ceiling to a colliding item.
 * \param that The other item in the collision.
 * \param info Informations on the collision.
 */
void bear::descending_ceiling::apply_angle_to
( engine::base_item& that, const universe::collision_info& info ) const
{
  double angle = std::atan(m_line.direction.y / get_width()) + 3.14159;

  that.set_contact_angle( angle );

  info.get_collision_repair().set_contact_normal
    (that, that.get_x_axis().get_orthonormal_anticlockwise());
} // descending_ceiling::apply_angle_to()
