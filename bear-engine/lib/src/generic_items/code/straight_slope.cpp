/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::straight_slope class.
 * \author Julien Jorge
 */
#include "generic_items/straight_slope.hpp"

#include "universe/collision_align_policy.hpp"
#include "universe/collision_info.hpp"
#include "engine/export.hpp"

BASE_ITEM_EXPORT( straight_slope, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::straight_slope::collision_result::collision_result()
{

} // straight_slope::collision_result::collision_result()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param a Tell if the other item has been aligned.
 * \param f The friction of the side collided by the item.
 */
bear::straight_slope::collision_result::collision_result( bool a, double f )
  : align(a), friction(f)
{

} // straight_slope::collision_result::collision_result()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::straight_slope::straight_slope()
  : m_margin(5), m_top_side_is_active(false), m_bottom_side_is_active(false)
{
  set_weak_collisions(false);
} // straight_slope::straight_slope()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::straight_slope::set_bool_field
( const std::string& name, bool value )
{
  bool ok(true);

  if (name == "straight_slope.top_side_is_active")
    m_top_side_is_active = value;
  else if (name == "straight_slope.bottom_side_is_active")
    m_bottom_side_is_active = value;
  else
    ok = super::set_bool_field(name, value);

  return ok;
} // straight_slope::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c <real>.
 * \param name The name of the field to set.
 * \param value The new value of the field.
 */
bool bear::straight_slope::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "straight_slope.margin" )
    m_margin = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // straight_slope::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::straight_slope::build()
{
  super::build();

  if (m_top_side_is_active)
    set_height( get_height() + m_margin );

  if (m_bottom_side_is_active)
    {
      set_height( get_height() + m_margin );
      set_bottom( get_bottom() - m_margin );

      claw::math::coordinate_2d<int> gap(get_gap());
      gap.y += m_margin;
      set_gap(gap);
    }

  init_default_contact_mode
    ( m_top_side_is_active, m_bottom_side_is_active, false, false );
} // straight_slope::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the center of the other item is included in the bounds
 * of \a this before processing the collision.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::straight_slope::collision_check_center_included
( engine::base_item& that, universe::collision_info& info )
{
  const universe::coordinate_type pos_x
    ( that.get_horizontal_middle() );
  const universe::position_type prev_bottom_middle
    ( info.other_previous_state().get_bottom_middle() );
  const universe::coordinate_type prev_top
    ( info.other_previous_state().get_top() );

  if ( (pos_x >= get_left()) && (pos_x <= get_right()) )
    {
      collision_result result(false);

      if ( (prev_bottom_middle.x >= get_left())
           && (prev_bottom_middle.x <= get_right()) )
        {
          if ( (prev_bottom_middle.y >= get_top() - m_margin)
               && (that.get_bottom() < get_top() - m_margin) )
            result = collision_classic_ground(that, info);
          else if ( (that.get_top() > get_bottom())
                    && (prev_top <= get_bottom()) )
            result = collision_classic_ceiling(that, info);
        }
      else if ( (that.get_bottom() <= get_top())
                && (prev_bottom_middle.y >= get_top() - 2 * m_margin) )
        result = collision_slope_ground(that, info);
      else if ( (that.get_top() >= get_bottom())
                && (prev_top <= get_bottom() + 2 * m_margin) )
        result = collision_slope_ceiling(that, info);

      if (result.align)
        {
          that.set_contact_friction(result.friction);
          that.set_contact_angle(0);
          do_z_shift(that);
        }
    }
} // straight_slope::collision_check_center_include()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item just like a classic block.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bear::straight_slope::collision_result
bear::straight_slope::collision_classic_ground
( engine::base_item& that, universe::collision_info& info )
{
  if ( !m_top_side_is_active )
    return collision_result(false);

  const universe::position_type pos( that.get_left(), get_top() - m_margin );

  const universe::collision_align_policy policy
    ( get_top_contact_mode( info, pos ) );

  return collision_result
    ( collision_align_top(info, pos, policy), get_top_friction() );
} // straight_slope::collision_classic_ground()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item just like a classic ceiling.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bear::straight_slope::collision_result
bear::straight_slope::collision_classic_ceiling
( engine::base_item& that, universe::collision_info& info )
{
  if ( !m_bottom_side_is_active )
    return collision_result(false);

  const universe::position_type pos
    ( that.get_left(), get_bottom() + m_margin - that.get_height() );

  const universe::collision_align_policy policy
    ( get_bottom_contact_mode( info, pos ) );

  return collision_result
    ( collision_align_bottom(info, pos, policy), get_bottom_friction() );
} // straight_slope::collision_classic_ceiling()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item when it comes from an adjacent slope.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bear::straight_slope::collision_result
bear::straight_slope::collision_slope_ground
( engine::base_item& that, universe::collision_info& info )
{
  if ( !m_top_side_is_active )
    return collision_result(false);

  const universe::position_type pos( that.get_left(), get_top() - m_margin );

  universe::speed_type s = that.get_speed();

  if ( s.y > 0 )
    {
      s.y = 0;
      that.set_speed(s);
    }

  const universe::collision_align_policy policy
    ( get_top_contact_mode( info, pos ) );

  return collision_result
    ( collision_align_top(info, pos, policy), get_top_friction() );
} // straight_slope::collision_slope_ground()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item when it comes from an adjacent ceiling slope.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bear::straight_slope::collision_result
bear::straight_slope::collision_slope_ceiling
( engine::base_item& that, universe::collision_info& info )
{
  if ( !m_bottom_side_is_active )
    return collision_result(false);

  const universe::position_type pos
    ( that.get_left(), get_bottom() + m_margin - that.get_height() );

  universe::speed_type s = that.get_speed();

  if ( s.y < 0 )
    {
      s.y = 0;
      that.set_speed(s);
    }

  const universe::collision_align_policy policy
    ( get_bottom_contact_mode( info, pos ) );

  return collision_result
    ( collision_align_bottom(info, pos, policy), get_bottom_friction() );
} // straight_slope::collision_slope_ceiling()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call collision_check_center_included().
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::straight_slope::collision
( engine::base_item& that, universe::collision_info& info )
{
  if ( satisfy_collision_condition(that) )
    collision_check_center_included(that, info);
} // straight_slope::collision()
