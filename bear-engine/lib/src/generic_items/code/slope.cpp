/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::slope class.
 * \author Julien Jorge
 */
#include "generic_items/slope.hpp"

#include "universe/collision_align_policy.hpp"
#include "universe/collision_info.hpp"
#include "universe/collision_repair.hpp"

#include "universe/shape/curved_box.hpp"
#include "universe/shape/shape_traits.hpp"

#include "engine/layer/layer.hpp"
#include "engine/world.hpp"

BASE_ITEM_EXPORT( slope, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item loaded by this loader.
 */
bear::slope::loader::loader( slope& item )
: super( "slope" ), m_item( item )
{

} // slope::loader::loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instantiates a copy of this instance.
 */
bear::slope::loader* bear::slope::loader::clone() const
{
  return new loader( *this );
} // slope::loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c <real>.
 * \param name The name of the field to set.
 * \param value The new value of the field.
 */
bool bear::slope::loader::set_field( const std::string& name, double value )
{
  bool result = true;

  if ( name == "tangent_friction" )
    m_item.m_tangent_friction = value;
  else
    {
      // loads the fields that describe the shape
      universe::curved_box* c = m_item.get_curved_box();

      if ( c == NULL )
        return false;

      if ( name == "steepness" )
        c->set_steepness( value );
      else if ( name == "control_point.left.x" )
        c->set_left_control_point
          ( universe::vector_type( value, c->get_left_control_point().y ) );
      else if ( name == "control_point.left.y" )
        c->set_left_control_point
          ( universe::vector_type( c->get_left_control_point().x, value ) );
      else if ( name == "control_point.right.x" )
        c->set_right_control_point
          ( universe::vector_type( value, c->get_right_control_point().y ) );
      else if ( name == "control_point.right.y" )
        c->set_right_control_point
          ( universe::vector_type( c->get_right_control_point().x, value ) );
      else if ( name == "margin" )
        c->set_margin( value );
      else
        result = false;

      m_item.set_shape( *c );
      delete c;
    }

  if ( !result )
    result = super::set_field(name, value);

  return result;
} // slope::loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field to set.
 * \param value The new value of the field.
 */
bool bear::slope::loader::set_field( const std::string& name, bool value )
{
  bool result = true;

  if ( name == "opposite_side_is_active" )
    m_item.m_opposite_side_is_active = value;
  else if ( name == "left_side_is_active" )
    m_item.m_left_side_is_active = value;
  else if ( name == "right_side_is_active" )
    m_item.m_right_side_is_active = value;
  else if ( name == "apply_angle" )
    m_item.m_apply_angle = value;
  else
    result = super::set_field(name, value);

  return result;
} // slope::loader::set_field()




/*----------------------------------------------------------------------------*/
const bear::universe::coordinate_type bear::slope::s_line_width = 10;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::slope::slope()
  : m_tangent_friction(0.8), m_opposite_side_is_active(false),
    m_left_side_is_active(false), m_right_side_is_active(false),
    m_apply_angle(true)
{
  set_shape( universe::curved_box() );

  set_weak_collisions(false);
} // slope::slope()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::slope::build()
{
  // We remove the margin during the build of the parent classes to be sure that
  // the automatic size is computed if no size has been provided for the slope.
  universe::curved_box* const c( get_curved_box() );

  universe::size_type previous_margin( c->get_margin() );
  c->set_margin( 0 );
  set_shape( *c );

  super::build();

  universe::shape_traits<universe::curved_box>::set_size( *c, get_size() );
  c->set_margin( previous_margin );
  set_shape( *c );

  delete c;

  init_default_contact_mode
    ( true, m_opposite_side_is_active, m_left_side_is_active,
      m_right_side_is_active );
} // slope::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the curve describing the slope.
 */
bear::slope::curve_type bear::slope::get_curve() const
{
  curve_type result;

  const universe::curved_box* const c( get_curved_box() );

  if ( c != NULL )
    result = c->get_curve();

  delete c;

  return result;
} // slope::get_curve()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the Y-coordinate on the slope according to a given X-coordinate.
 * \param x The considered X-coordinate.
 */
bear::universe::coordinate_type 
bear::slope::get_y_at_x( universe::coordinate_type x ) const
{
  const universe::curved_box* const c( get_curved_box() );

  const universe::coordinate_type result
    ( (c == NULL) ? get_bottom() : c->get_y_at_x(x) );
  delete c;

  return result;
} // slope::get_y_at_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the steepness of the slope.
 */
bear::universe::coordinate_type bear::slope::get_steepness() const
{
  const universe::curved_box* const c( get_curved_box() );

  const universe::coordinate_type result
    ( (c == NULL) ? 0 : c->get_steepness() );
  delete c;

  return result;
} // slope::get_steepness()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::slope::populate_loader_map( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( loader(*this) );
} // slope::populate_loader_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::slope::collision_as_slope
( engine::base_item& that, universe::collision_info& info )
{
  double f(1);
  bool align_as_block(false);
  bool align_as_slope(false);

  switch( info.get_collision_side() )
    {
    case universe::zone::bottom_zone:
      f = get_bottom_friction();
      align_as_block = m_opposite_side_is_active;
      break;
    case universe::zone::top_zone:
      f = get_top_friction();
      align_as_slope = true;
      break;
    case universe::zone::middle_left_zone:
      f = get_left_friction();
      if ( check_left_contact_as_slope(that, info) )
        align_as_slope = true;
      else
        align_as_block = m_left_side_is_active;
      break;
    case universe::zone::middle_right_zone:
      f = get_right_friction();
      if ( check_right_contact_as_slope(that, info) )
        align_as_slope = true;
      else
        align_as_block = m_right_side_is_active;
      break;
    case universe::zone::middle_zone:
      f = get_top_friction();
      if ( check_bottom_above_ground(that, info) )
        align_as_slope = true;
      break;
    default: { CLAW_ASSERT( false, "Invalid collision side." ); }
    }

  bool aligned(false);

  if ( satisfy_collision_condition(that) )
    {
      if ( align_as_slope )
        aligned = align_on_ground(that, info);
      else if ( align_as_block )
        aligned = default_collision(info);
      else
        aligned = align_nearest_edge(that, info);

      if (aligned)
        {
          that.set_contact_friction(f);
          do_z_shift(that);
        }
    }
} // slope::collision_as_slope()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call collision_as_slope().
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::slope::collision
( engine::base_item& that, universe::collision_info& info )
{
  collision_as_slope(that, info);
} // slope::collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if an item colliding on the left is aligned on the ground.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::slope::check_left_contact_as_slope
( engine::base_item& that, universe::collision_info& info ) const
{
  bool result = false;

  // the slope goes from top right to bottom left
  if ( get_steepness() > 0 )
    result = info.get_bottom_left_on_contact().y
      >= get_y_at_x( get_left() ) - s_line_width;
  
  return result;
} // slope::check_left_contact_as_slope()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if an item colliding on the right is aligned on the ground.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::slope::check_right_contact_as_slope
( engine::base_item& that, universe::collision_info& info ) const
{
  bool result = false;

  // the slope goes from top left to bottom right
  if ( get_steepness() < 0 )
    result = info.get_bottom_left_on_contact().y
      >= get_y_at_x( get_right() ) - s_line_width;

  return result;
} // slope::check_right_contact_as_slope()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the bottom of the other item was above the ground at
 *        collision time.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::slope::check_bottom_above_ground
( engine::base_item& that, universe::collision_info& info ) const
{
  const universe::position_type pos
    (info.other_previous_state().get_bottom_middle());
  
  return pos.y >= get_y_at_x( pos.x ) - s_line_width;
} // slope::check_bottom_above_ground()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align \a that on the ground line.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::slope::align_on_ground
( engine::base_item& that, universe::collision_info& info )
{
  bool result = false;

  const universe::coordinate_type pos_x
    ( info.get_bottom_left_on_contact().x + that.get_width() / 2 );
  
  if ( (pos_x >= get_left()) && (pos_x <= get_right())
       && item_crossed_up_down(that, info) )
    {
      universe::position_type pos
        ( info.get_bottom_left_on_contact().x, get_y_at_x( pos_x ) );
  
      if ( std::abs(pos.x - info.other_previous_state().get_left()) +
           std::abs(pos.y - info.other_previous_state().get_bottom()) < 0.6)
        pos = info.other_previous_state().get_bottom_left();

      const universe::collision_align_policy policy
        ( get_top_contact_mode(info, pos) );

      if ( collision_align_top(info, pos, policy) )
        {
          result = true;

          if ( m_apply_angle )
            apply_angle_to(that, info);
        }
    }

  return result;
} // slope::align_on_ground()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align \a that on the nearest vertical edge.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::slope::align_nearest_edge
( engine::base_item& that, universe::collision_info& info )
{
  bool result = false;

  if ( info.reference_previous_state().get_center_of_mass().x
       <= info.other_previous_state().get_center_of_mass().x )
    {
      if (m_right_side_is_active)
        {
          const universe::collision_align_policy policy
            ( get_right_contact_mode(info) );

          result = collision_align_right(info, policy);
        }
    }
  else if (m_left_side_is_active)
    {
      const universe::collision_align_policy policy
        ( get_left_contact_mode(info) );

      result = collision_align_left(info, policy);
    }

  return result;
} // slope::align_nearest_edge()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the other item has crossed the surface following a top to down
 *        direction.
 * \param that The other item in the collision.
 * \param info Informations on the collision.
 */
bool bear::slope::item_crossed_up_down
( engine::base_item& that, const universe::collision_info& info ) const
{
  bool result = false;

  if ( that.get_bottom()
       <= get_y_at_x( that.get_center_of_mass().x ) + s_line_width )
    {
      const universe::position_type other_prev_bottom
        ( info.other_previous_state().get_bottom_middle() );

      if ( other_prev_bottom.x < get_left() )
        result =
          other_prev_bottom.y >= get_y_at_x( get_left() ) - s_line_width;
      else if ( other_prev_bottom.x > get_right() )
        result =
          other_prev_bottom.y >= get_y_at_x( get_right() ) - s_line_width;
      else
        result = other_prev_bottom.y >=
          get_y_at_x( other_prev_bottom.x ) - s_line_width;
    }

  return result;
} // slope::item_crossed_up_down()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the angle of the slope to a colliding item.
 * \param that The other item in the collision.
 * \param info Informations on the collision.
 */
void bear::slope::apply_angle_to
( engine::base_item& that, const universe::collision_info& info ) const
{
  const universe::coordinate_type pos_x
    ( info.get_bottom_left_on_contact().x + that.get_width() / 2 );
  
  const curve_type::section::resolved_point p =
    get_curve().get_point_at_x( pos_x - get_left() )[0];
  bear::universe::position_type tangent
    ( p.get_section().get_tangent_at(p.get_date()) );
  
  double angle = std::atan2( tangent.y, tangent.x );
  
  that.set_contact_angle( angle);

  universe::coordinate_type g_force(0);

  if ( get_layer().has_world() )
    g_force = std::abs(get_layer().get_world().get_gravity().y
                       * info.other_previous_state().get_mass()
                       + info.other_previous_state().get_force().y);

  const universe::coordinate_type normal_length =
    g_force * std::cos(angle);
  const universe::coordinate_type x_gravity_length =
    g_force * std::sin(angle);
  const universe::coordinate_type friction_length =
    normal_length * m_tangent_friction;

  if ( x_gravity_length > friction_length )
    {
      const universe::coordinate_type d =
        x_gravity_length - friction_length;

      if ( get_steepness() > 0 )
        that.add_internal_force( universe::force_type(-d, 0) );
      else
        that.add_internal_force( universe::force_type(d, 0) );
    }
  else
    that.add_internal_force
      ( universe::force_type(x_gravity_length, 0) );

  info.get_collision_repair().set_contact_normal
    (that, that.get_x_axis().get_orthonormal_anticlockwise());
} // slope::apply_angle_to()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the curve describing the slope.
 */
bear::universe::curved_box* bear::slope::get_curved_box() const
{
  universe::shape_base* const s( get_shape().clone_impl() );

  universe::curved_box* const c
    ( dynamic_cast<universe::curved_box*>( s ) );

  if ( c == NULL )
    {
      delete s;
      return NULL;
    }

  return c;
} // slope::get_curved_box()
