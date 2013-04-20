/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::physical_item_state class.
 * \author Julien Jorge
 */
#include "universe/physical_item_state.hpp"

#include "universe/shape/shape_traits.hpp"

#include <sstream>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::physical_item_state::physical_item_state()
  : m_fixed(false)
{

} // physical_item_state::physical_item_state()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 *
 * By default, this copy is not fixed.
 */
bear::universe::physical_item_state::physical_item_state
( const physical_item_state& that )
  : m_attributes(that.m_attributes), m_fixed(false)
{

} // physical_item_state::physical_item_state()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::universe::physical_item_state::~physical_item_state()
{
  // nothing to do
} // physical_item_state::physical_item_state()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the item.
 */
bear::universe::size_box_type
bear::universe::physical_item_state::get_size() const
{
  return shape_traits<shape>::get_size( m_attributes.m_shape );
} // physical_item_state::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of the item.
 */
bear::universe::size_type bear::universe::physical_item_state::get_width() const
{
  return shape_traits<shape>::get_width( m_attributes.m_shape );
} // physical_item_state::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of the item.
 */
bear::universe::size_type
bear::universe::physical_item_state::get_height() const
{
  return shape_traits<shape>::get_height( m_attributes.m_shape );
} // physical_item_state::get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the bouding box of this item.
 * \param r the new bounding box of the item.
 */
void bear::universe::physical_item_state::set_bounding_box
( const bear::universe::rectangle_type& r )
{
  set_bottom_left( r.bottom_left() );
  set_size( r.size() );
} // physical_item_state::set_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the bouding box of this item.
 */
bear::universe::rectangle_type
bear::universe::physical_item_state::get_bounding_box() const
{
  return shape_traits<shape>::get_bounding_box( m_attributes.m_shape );
} // physical_item_state::get_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current acceleration of the item.
 */
const bear::universe::force_type&
bear::universe::physical_item_state::get_acceleration() const
{
  return m_attributes.m_acceleration;
} // physical_item_state::get_acceleration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the acceleration of the item.
 */
void bear::universe::physical_item_state::set_acceleration(const force_type& a)
{
  if (!m_fixed)
    {
      if (m_attributes.m_x_fixed == 0)
        m_attributes.m_acceleration.x = a.x;

      if (m_attributes.m_y_fixed == 0)
        m_attributes.m_acceleration.y = a.y;
    }
} // physical_item_state::set_acceleration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current total force applied to the item.
 */
bear::universe::force_type
bear::universe::physical_item_state::get_force() const
{
  const vector_type x_axis(get_x_axis());

  return m_attributes.m_external_force
    + m_attributes.m_internal_force.x * x_axis
    + m_attributes.m_internal_force.y * x_axis.get_orthonormal_anticlockwise();
} // physical_item_state::get_force()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current internal force applied to the item.
 */
const bear::universe::force_type&
bear::universe::physical_item_state::get_internal_force() const
{
  return m_attributes.m_internal_force;
} // physical_item_state::get_internal_force()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current external force applied to the item.
 */
const bear::universe::force_type&
bear::universe::physical_item_state::get_external_force() const
{
  return m_attributes.m_external_force;
} // physical_item_state::get_external_force()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply one more internal force to the item.
 * \param force The force to apply.
 */
void bear::universe::physical_item_state::add_internal_force
( const force_type& force )
{
  if (!m_fixed)
    {
      if (m_attributes.m_x_fixed == 0)
        m_attributes.m_internal_force.x += force.x;

      if (m_attributes.m_y_fixed == 0)
        m_attributes.m_internal_force.y += force.y;
    }
} // physical_item_state::add_internal_force()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply one more external force to the item.
 * \param force The force to apply.
 */
void bear::universe::physical_item_state::add_external_force
( const force_type& force )
{
  if (!m_fixed)
    {
      if (m_attributes.m_x_fixed == 0)
        m_attributes.m_external_force.x += force.x;

      if (m_attributes.m_y_fixed == 0)
        m_attributes.m_external_force.y += force.y;
    }
} // physical_item_state::add_external_force()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the external force of the item.
 */
void bear::universe::physical_item_state::set_external_force
(const force_type& f)
{
  if (!m_fixed)
    {
      if (m_attributes.m_x_fixed == 0)
        m_attributes.m_external_force.x = f.x;

      if (m_attributes.m_y_fixed == 0)
        m_attributes.m_external_force.y = f.y;
    }
} // physical_item_state::set_external_force()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the internal force of the item.
 */
void bear::universe::physical_item_state::set_internal_force
(const force_type& f)
{
  if (!m_fixed)
    {
      if (m_attributes.m_x_fixed == 0)
        m_attributes.m_internal_force.x = f.x;

      if (m_attributes.m_y_fixed == 0)
        m_attributes.m_internal_force.y = f.y;
    }
} // physical_item_state::set_internal_force()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the mass of the item.
 */
double bear::universe::physical_item_state::get_mass() const
{
  return m_attributes.m_mass;
} // physical_item_state::get_mass()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the density of the item.
 */
double bear::universe::physical_item_state::get_density() const
{
  return m_attributes.m_density;
} // physical_item_state::get_density()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current speed of the item.
 */
const bear::universe::speed_type&
bear::universe::physical_item_state::get_speed() const
{
  return m_attributes.m_speed;
} // physical_item_state::get_speed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the current speed of the item.
 * \param speed The speed.
 */
void bear::universe::physical_item_state::set_speed( const speed_type& speed )
{
  set_speed( speed.x, speed.y );
} // physical_item_state::set_speed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the current speed of the item.
 * \param x The speed on the x-axis.
 * \param y The speed on the y-axis.
 */
void bear::universe::physical_item_state::set_speed( double x, double y )
{
  if (!m_fixed)
    {
      if (m_attributes.m_x_fixed == 0)
        m_attributes.m_speed.x = x;

      if (m_attributes.m_y_fixed == 0)
        m_attributes.m_speed.y = y;
    }
} // physical_item_state::set_speed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current angular speed of the item.
 */
double bear::universe::physical_item_state::get_angular_speed() const
{
  return m_attributes.m_angular_speed;
} // physical_item_state::get_angular_speed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the current angular speed of the item.
 * \param The new angular speed.
 */
void bear::universe::physical_item_state::set_angular_speed( double speed )
{
  m_attributes.m_angular_speed = speed;
} // physical_item_state::set_angular_speed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a speed at the current angular speed of the item.
 * \param speed The angular speed to add.
 */
void bear::universe::physical_item_state::add_angular_speed( double speed )
{
  m_attributes.m_angular_speed += speed;
} // physical_item_state::set_angular_speed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the friction applied to this item.
 */
double bear::universe::physical_item_state::get_friction() const
{
  return m_attributes.m_self_friction;
} // physical_item_state::get_friction();

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the friction applied to this item.
 * \param f The value of the friction applied to this item.
 */
void bear::universe::physical_item_state::set_friction( double f )
{
  m_attributes.m_self_friction = f;
} // physical_item_state::set_friction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the friction applied to this item by a contact with an other item.
 */
double bear::universe::physical_item_state::get_contact_friction() const
{
  return m_attributes.m_contact_friction;
} // physical_item_state::get_contact_friction();

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the friction applied to this item by a contact with an other item.
 * \param f The value of the friction applied to this item.
 */
void bear::universe::physical_item_state::set_contact_friction( double f )
{
  m_attributes.m_contact_friction = f;
} // physical_item_state::set_contact_friction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the elasticity of this item.
 */
double bear::universe::physical_item_state::get_elasticity() const
{
  return m_attributes.m_elasticity;
} // physical_item_state::get_elasticity();

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the elasticity of this item.
 * \param e The value of the elasticity of this item.
 */
void bear::universe::physical_item_state::set_elasticity( double e )
{
  m_attributes.m_elasticity = e;
} // physical_item_state::set_elasticity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the hardness of this item.
 */
double bear::universe::physical_item_state::get_hardness() const
{
  return m_attributes.m_hardness;
} // physical_item_state::get_hardness();

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the hardness of this item.
 * \param h The value of the hardness of this item.
 */
void bear::universe::physical_item_state::set_hardness( double h )
{
  m_attributes.m_hardness = h;
} // physical_item_state::set_hardness()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the angle of this item.
 */
double bear::universe::physical_item_state::get_system_angle() const
{
  return m_attributes.m_system_angle;
} // physical_item_state::get_system_angle();

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the angle of this item.
 * \param a The value of the angle.
 */
void bear::universe::physical_item_state::set_system_angle( double a )
{
  m_attributes.m_system_angle = a;
} // physical_item_state::set_system_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the angle of this item on a contact.
 * \param a The value of the angle.
 */
void bear::universe::physical_item_state::set_contact_angle( double a )
{
  if ( !has_free_system() )
    m_attributes.m_system_angle = a;
} // physical_item_state::set_contact_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the system angle of the item is not modified by the
 *        collisions.
 */
bool bear::universe::physical_item_state::has_free_system() const
{
  return m_attributes.m_free_system_angle;
} // physical_item_state::has_free_system();

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the system angle of the item is not modified by the
 *        collisions.
 * \param b True if the system angle of the item is not modified by the
 *        collisions.
 */
void bear::universe::physical_item_state::set_free_system( bool b )
{
  m_attributes.m_free_system_angle = b;
} // physical_item_state::set_free_system();

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the local x-axis of the item.
 */
bear::universe::vector_type
bear::universe::physical_item_state::get_x_axis() const
{
  return vector_type
    ( std::cos(m_attributes.m_system_angle),
      std::sin(m_attributes.m_system_angle) );
} // physical_item_state::get_x_axis()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the top edge of the item.
 * \param pos The new position.
 */
void bear::universe::physical_item_state::set_top( coordinate_type pos )
{
  set_bottom( pos - get_height() );
} // physical_item_state::set_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the bottom edge of the item.
 * \param pos The new position.
 */
void bear::universe::physical_item_state::set_bottom( coordinate_type pos )
{
  if (!m_fixed && (m_attributes.m_y_fixed == 0))
    shape_traits<shape>::set_bottom( m_attributes.m_shape, pos );
} // physical_item_state::set_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the left edge of the item.
 * \param pos The new position.
 */
void bear::universe::physical_item_state::set_left( coordinate_type pos )
{
  if (!m_fixed && (m_attributes.m_x_fixed == 0))
    shape_traits<shape>::set_left( m_attributes.m_shape, pos );
} // physical_item_state::set_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the right edge of the item.
 * \param pos The new position.
 */
void bear::universe::physical_item_state::set_right( coordinate_type pos )
{
  set_left( pos - get_width() );
} // physical_item_state::set_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the x-position of the center of the item.
 * \param pos The new position.
 */
void bear::universe::physical_item_state::set_horizontal_middle
( coordinate_type pos )
{
  set_left( pos - get_width() / 2 );
} // physical_item_state::set_horizontal_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the y-position of the center of the item.
 * \param pos The new position.
 */
void bear::universe::physical_item_state::set_vertical_middle
( coordinate_type pos )
{
  set_bottom( pos - get_height() / 2 );
} // physical_item_state::set_vertical_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the top left corner of the item.
 * \param pos The new position.
 */
void
bear::universe::physical_item_state::set_top_left( const position_type& pos )
{
  set_left(pos.x);
  set_top(pos.y);
} // physical_item_state::set_top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the middle of the top edge of the item.
 * \param pos The new position.
 */
void
bear::universe::physical_item_state::set_top_middle( const position_type& pos )
{
  set_horizontal_middle(pos.x);
  set_top(pos.y);
} // physical_item_state::set_top_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the top right corner of the item.
 * \param pos The new position.
 */
void
bear::universe::physical_item_state::set_top_right( const position_type& pos )
{
  set_right(pos.x);
  set_top(pos.y);
} // physical_item_state::set_top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the bottom left corner of the item.
 * \param pos The new position.
 */
void
bear::universe::physical_item_state::set_bottom_left( const position_type& pos )
{
  set_left(pos.x);
  set_bottom(pos.y);
} // physical_item_state::set_bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the middle of the bottom edge of the item.
 * \param pos The new position.
 */
void bear::universe::physical_item_state::set_bottom_middle
( const position_type& pos )
{
  set_horizontal_middle(pos.x);
  set_bottom(pos.y);
} // physical_item_state::set_bottom_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the bottom right corner of the item.
 * \param pos The new position.
 */
void bear::universe::physical_item_state::set_bottom_right
( const position_type& pos )
{
  set_right(pos.x);
  set_bottom(pos.y);
} // physical_item_state::set_bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the middle of the left edge of the item.
 * \param pos The new position.
 */
void
bear::universe::physical_item_state::set_left_middle( const position_type& pos )
{
  set_left(pos.x);
  set_vertical_middle(pos.y);
} // physical_item_state::set_left_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the middle of the right edge of the item.
 * \param pos The new position.
 */
void bear::universe::physical_item_state::set_right_middle
( const position_type& pos )
{
  set_right(pos.x);
  set_vertical_middle(pos.y);
} // physical_item_state::set_right_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the top left corner of the item.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::physical_item_state::set_top_left
( const coordinate_type& x, const coordinate_type& y )
{
  set_left(x);
  set_top(y);
} // physical_item_state::set_top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the middle of the top edge of the item.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::physical_item_state::set_top_middle
( const coordinate_type& x, const coordinate_type& y )
{
  set_horizontal_middle(x);
  set_top(y);
} // physical_item_state::set_top_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the top right corner of the item.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::physical_item_state::set_top_right
( const coordinate_type& x, const coordinate_type& y )
{
  set_right(x);
  set_top(y);
} // physical_item_state::set_top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the bottom left corner of the item.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::physical_item_state::set_bottom_left
( const coordinate_type& x, const coordinate_type& y )
{
  set_left(x);
  set_bottom(y);
} // physical_item_state::set_bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the middle of the bottom edge of the item.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::physical_item_state::set_bottom_middle
( const coordinate_type& x, const coordinate_type& y )
{
  set_horizontal_middle(x);
  set_bottom(y);
} // physical_item_state::set_bottom_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the bottom right corner of the item.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::physical_item_state::set_bottom_right
( const coordinate_type& x, const coordinate_type& y )
{
  set_right(x);
  set_bottom(y);
} // physical_item_state::set_bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the middle of the left edge of the item.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::physical_item_state::set_left_middle
( const coordinate_type& x, const coordinate_type& y )
{
  set_left(x);
  set_vertical_middle(y);
} // physical_item_state::set_left_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the middle of the right edge of the item.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::physical_item_state::set_right_middle
( const coordinate_type& x, const coordinate_type& y )
{
  set_right(x);
  set_vertical_middle(y);
} // physical_item_state::set_right_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the lowest X-coordinate covered by the bounding box of this item.
 */
bear::universe::coordinate_type
bear::universe::physical_item_state::get_left() const
{
  return shape_traits<shape>::get_left( m_attributes.m_shape );
} // physical_item_state::get_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the lowest Y-coordinate covered by the bounding box of this item.
 */
bear::universe::coordinate_type
bear::universe::physical_item_state::get_top() const
{
  return get_bottom() + get_height();
} // physical_item_state::get_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the highest X-coordinate covered by the bounding box of this item.
 */
bear::universe::coordinate_type
bear::universe::physical_item_state::get_right() const
{
  return get_left() + get_width();
} // physical_item_state::get_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the highest Y-coordinate covered by the bounding box of this item.
 */
bear::universe::coordinate_type
bear::universe::physical_item_state::get_bottom() const
{
  return shape_traits<shape>::get_bottom( m_attributes.m_shape );
} // physical_item_state::get_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the x-coordinate of the center of this item.
 */
bear::universe::coordinate_type
bear::universe::physical_item_state::get_horizontal_middle() const
{
  return get_left() + get_width() / 2;
} // physical_item_state::get_horizontal_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the Y-coordinate of the center of this item.
 */
bear::universe::coordinate_type
bear::universe::physical_item_state::get_vertical_middle() const
{
  return get_bottom() + get_height() / 2;
} // physical_item_state::get_vertical_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the top left corner of the item.
 */
bear::universe::position_type
bear::universe::physical_item_state::get_top_left() const
{
  return position_type( get_left(), get_top() );
} // physical_item_state::get_top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the middle of the top edge of the item.
 */
bear::universe::position_type
 bear::universe::physical_item_state::get_top_middle() const
{
  return position_type( get_center_of_mass().x, get_top() );
} // physical_item_state::get_top_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the top right corner of the item.
 */
bear::universe::position_type
bear::universe::physical_item_state::get_top_right() const
{
  return position_type( get_right(), get_top() );
} // physical_item_state::get_top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the bottom left corner of the item.
 */
bear::universe::position_type
bear::universe::physical_item_state::get_bottom_left() const
{
  return position_type( get_left(), get_bottom() );
} // physical_item_state::get_bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the middle of the bottom edge of the item.
 */
bear::universe::position_type
bear::universe::physical_item_state::get_bottom_middle() const
{
  return position_type( get_center_of_mass().x, get_bottom() );
} // physical_item_state::get_bottom_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the bottom right corner of the item.
 */
bear::universe::position_type
bear::universe::physical_item_state::get_bottom_right() const
{
  return position_type( get_right(), get_bottom() );
} // physical_item_state::get_bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the middle of the left edge of the item.
 */
bear::universe::position_type
bear::universe::physical_item_state::get_left_middle() const
{
  return position_type( get_left(), get_center_of_mass().y );
} // physical_item_state::get_left_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the middle of the right edge of the item.
 */
bear::universe::position_type
bear::universe::physical_item_state::get_right_middle() const
{
  return position_type( get_right(), get_center_of_mass().y );
} // physical_item_state::get_right_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position, in the world, of the center of mass.
 */
bear::universe::position_type
bear::universe::physical_item_state::get_center_of_mass() const
{
  return get_bottom_left() + get_size() / 2;
} // physical_item_state::get_center_of_mass()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the center of the item.
 * \param pos The new center of mass.
 */
void bear::universe::physical_item_state::set_center_of_mass
( const position_type& pos )
{
  set_bottom_left( pos - get_size() / 2 );
} // physical_item_state::set_center_of_mass()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the center of the item on the center of
 *        mass of an other item.
 * \param pos The item on which the center will be aligned.
 */
void bear::universe::physical_item_state::set_center_on
( const physical_item_state& that )
{
  set_center_of_mass( that.get_center_of_mass() );
} // physical_item_state::set_center_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set (force) the position of the center of the item.
 * \param x The new x_coordinate of the center of mass.
 * \param y The new y-coordinate of the center of mass.
 */
void bear::universe::physical_item_state::set_center_of_mass
( const coordinate_type& x, const coordinate_type& y )
{
  set_center_of_mass( position_type(x, y) );
} // physical_item_state::set_center_of_mass()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is fixed.
 */
bool bear::universe::physical_item_state::is_fixed() const
{
  return m_fixed;
} // physical_item_state::is_fixed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fix the item. Its speed and acceleration are set to zero. Its
 *        position, speed and acceleration won't be able to cange anymore.
 */
void bear::universe::physical_item_state::fix()
{
  m_attributes.m_acceleration = m_attributes.m_speed = speed_type(0, 0);
  m_fixed = true;
} // physical_item_state::fix()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a constraint on the position of the item.
 */
void bear::universe::physical_item_state::remove_position_constraints()
{
  remove_position_constraint_x();
  remove_position_constraint_y();
} // physical_item_state::remove_position_constraints()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a constraint on the X-position of the item.
 */
void bear::universe::physical_item_state::remove_position_constraint_x()
{
  --m_attributes.m_x_fixed;
} // physical_item_state::remove_position_constraint_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a constraint on the Y-position of the item.
 */
void bear::universe::physical_item_state::remove_position_constraint_y()
{
  --m_attributes.m_y_fixed;
} // physical_item_state::remove_position_constraint_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a constraint on the position of the item.
 */
void bear::universe::physical_item_state::add_position_constraints()
{
  add_position_constraint_x();
  add_position_constraint_y();
} // physical_item_state::add_position_constraint_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a constraint on the X-position of the item.
 */
void bear::universe::physical_item_state::add_position_constraint_x()
{
  ++m_attributes.m_x_fixed;
} // physical_item_state::add_position_constraint_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a constraint on the Y-position of the item.
 */
void bear::universe::physical_item_state::add_position_constraint_y()
{
  ++m_attributes.m_y_fixed;
} // physical_item_state::add_position_constraint_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item can move items.
 */
bool bear::universe::physical_item_state::can_move_items() const
{
  return m_attributes.m_can_move_items;
} // physical_item_state::can_move_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the field can_move_items.
 */
void bear::universe::physical_item_state::set_can_move_items(bool value)
{
  m_attributes.m_can_move_items = value;
} // physical_item_state::set_can_move_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this item is global or not. A global item is always considered
 *        in world::progress().
 */
bool bear::universe::physical_item_state::is_global() const
{
  return m_attributes.m_global;
} // physical_item_state::is_global()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the global status of this item. A global item is always considered
 *        in world::progress().
 * \param global The new global status.
 * \remark Changing the global status has no effect once the item has been added
 *         in a world.
 */
void bear::universe::physical_item_state::set_global( bool global )
{
  m_attributes.m_global = global;
} // physical_item_state::set_global()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item has a contact on its left.
 */
bool bear::universe::physical_item_state::has_left_contact() const
{
  return m_attributes.m_contact.has_left_contact();
} // physical_item_state::has_left_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the contact range on the left side of the item.
 */
bear::universe::contact_range
bear::universe::physical_item_state::get_left_contact() const
{
  return m_attributes.m_contact.get_left_contact();
} // physical_item_state::get_left_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item has a contact on its right.
 */
bool bear::universe::physical_item_state::has_right_contact() const
{
  return m_attributes.m_contact.has_right_contact();
} // physical_item_state::has_right_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the contact range on the right side of the item.
 */
bear::universe::contact_range
bear::universe::physical_item_state::get_right_contact() const
{
  return m_attributes.m_contact.get_right_contact();
} // physical_item_state::get_right_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item has a contact on its top.
 */
bool bear::universe::physical_item_state::has_top_contact() const
{
  return m_attributes.m_contact.has_top_contact();
} // physical_item_state::has_top_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the contact range on the top side of the item.
 */
bear::universe::contact_range
bear::universe::physical_item_state::get_top_contact() const
{
  return m_attributes.m_contact.get_top_contact();
} // physical_item_state::get_top_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item has a contact on its bottom.
 */
bool bear::universe::physical_item_state::has_bottom_contact() const
{
  return m_attributes.m_contact.has_bottom_contact();
} // physical_item_state::has_bottom_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the contact range on the bottom side of the item.
 */
bear::universe::contact_range
bear::universe::physical_item_state::get_bottom_contact() const
{
  return m_attributes.m_contact.get_bottom_contact();
} // physical_item_state::get_bottom_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item has a contact in middle zone.
 */
bool bear::universe::physical_item_state::has_middle_contact() const
{
  return m_attributes.m_contact.has_middle_contact();
} // physical_item_state::has_middle_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item has a contact.
 */
bool bear::universe::physical_item_state::has_contact() const
{
  return m_attributes.m_contact.has_contact();
} // physical_item_state::has_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item has a contact on one of his sides.
 */
bool bear::universe::physical_item_state::has_side_contact() const
{
  return m_attributes.m_contact.has_side_contact();
} // physical_item_state::has_side_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that the item has a contact on its left.
 * \param contact The new contact status.
 */
void bear::universe::physical_item_state::set_left_contact(bool contact)
{
  if ( contact )
    m_attributes.m_contact.set_left_contact( 0, 1 );
  else
    m_attributes.m_contact.clear_left_contact();
} // physical_item_state::set_left_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the contact range on the left side of the item.
 * \param bottom The coordinate of the bottom of the contact in the world's
 *        coordinates.
 * \param top The coordinate of the top of the contact in the world's
 *        coordinates.
 */
void bear::universe::physical_item_state::set_left_contact
( coordinate_type bottom, coordinate_type top )
{
  CLAW_PRECOND( bottom <= top );

  return m_attributes.m_contact.set_left_contact
    ( std::max(0.0, (bottom - get_bottom()) / get_height()),
      std::min(1.0, (top - get_bottom()) / get_height()) );
} // physical_item_state::set_left_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that the item has a contact on its right.
 * \param contact The new contact status.
 */
void bear::universe::physical_item_state::set_right_contact(bool contact)
{
  if ( contact )
    m_attributes.m_contact.set_right_contact( 0, 1 );
  else
    m_attributes.m_contact.clear_right_contact();
} // physical_item_state::set_right_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the contact range on the right side of the item.
 * \param bottom The coordinate of the bottom of the contact in the world's
 *        coordinates.
 * \param top The coordinate of the top of the contact in the world's
 *        coordinates.
 */
void bear::universe::physical_item_state::set_right_contact
( coordinate_type bottom, coordinate_type top )
{
  CLAW_PRECOND( bottom <= top );

  return m_attributes.m_contact.set_right_contact
    ( std::max(0.0, (bottom - get_bottom()) / get_height()),
      std::min(1.0, (top - get_bottom()) / get_height()) );
} // physical_item_state::set_right_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that the item has a contact on its top.
 * \param contact The new contact status.
 */
void bear::universe::physical_item_state::set_top_contact(bool contact)
{
  if ( contact )
    m_attributes.m_contact.set_top_contact( 0, 1 );
  else
    m_attributes.m_contact.clear_top_contact();
} // physical_item_state::set_top_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the contact range on the top side of the item.
 * \param left The coordinate of the left of the contact in the world's
 *        coordinates.
 * \param right The coordinate of the right of the contact in the world's
 *        coordinates.
 */
void bear::universe::physical_item_state::set_top_contact
( coordinate_type left, coordinate_type right )
{
  CLAW_PRECOND( left <= right );

  return m_attributes.m_contact.set_top_contact
    ( std::max(0.0, (left - get_left()) / get_width()),
      std::min(1.0, (right - get_left()) / get_width()) );
} // physical_item_state::set_top_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that the item has a contact on its bottom.
 * \param contact The new contact status.
 */
void bear::universe::physical_item_state::set_bottom_contact(bool contact)
{
  if ( contact )
    m_attributes.m_contact.set_bottom_contact( 0, 1 );
  else
    m_attributes.m_contact.clear_bottom_contact();
} // physical_item_state::set_bottom_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the contact range on the bottom side of the item.
 * \param left The coordinate of the left of the contact in the world's
 *        coordinates.
 * \param right The coordinate of the right of the contact in the world's
 *        coordinates.
 */
void bear::universe::physical_item_state::set_bottom_contact
( coordinate_type left, coordinate_type right )
{
  CLAW_PRECOND( left <= right );

  return m_attributes.m_contact.set_bottom_contact
    ( std::max(0.0, (left - get_left()) / get_width()),
      std::min(1.0, (right - get_left()) / get_width()) );
} // physical_item_state::set_bottom_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that the item has a contact in middle zone.
 * \param contact The new contact status.
 */
void bear::universe::physical_item_state::set_middle_contact(bool contact)
{
  m_attributes.m_contact.set_middle_contact( contact );
} // physical_item_state::set_middle_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that the item has not any contact.
 */
void bear::universe::physical_item_state::clear_contacts()
{
  m_attributes.m_contact.clear_contacts();
  m_attributes.m_contact_friction = 1;
} // physical_item_state::clear_contacts()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the phantom status.
 * \param phantom The new status.
 */
void bear::universe::physical_item_state::set_phantom( bool phantom )
{
  m_attributes.m_is_phantom = phantom;
} // physical_item_state::set_phantom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the phantom status.
 */
bool bear::universe::physical_item_state::is_phantom() const
{
  return m_attributes.m_is_phantom;
} // physical_item_state::is_phantom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the artificial status.
 * \param a The new status.
 */
void bear::universe::physical_item_state::set_artificial( bool a )
{
  m_attributes.m_is_artificial = a;
} // physical_item_state::set_artificial()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the artificial status.
 */
bool bear::universe::physical_item_state::is_artificial() const
{
  return m_attributes.m_is_artificial;
} // physical_item_state::is_artificial()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indicate if the item is considered for weak or strong collisions.
 * \param w The new status.
 */
void bear::universe::physical_item_state::set_weak_collisions( bool w )
{
  m_attributes.m_weak_collisions = w;
} // physical_item_state::set_weak_collisions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item has to be considered as weak collisions.
 * \param w The new status.
 */
bool bear::universe::physical_item_state::has_weak_collisions() const
{
  return m_attributes.m_weak_collisions;
} // physical_item_state::has_weak_collisions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size of the object.
 * \param size The new size.
 */
void bear::universe::physical_item_state::set_size( const size_box_type& size )
{
  set_width( size.x );
  set_height( size.y );
} // physical_item_state::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size of the object.
 * \param width The width of the object.
 * \param height The height of the object.
 */
void bear::universe::physical_item_state::set_size
( size_type width, size_type height )
{
  set_size( size_box_type(width, height) );
} // physical_item_state::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the width of the object.
 * \param width The width of the object.
 */
void bear::universe::physical_item_state::set_width( size_type width )
{
  if (!m_fixed && (m_attributes.m_x_fixed == 0))
    shape_traits<shape>::set_width( m_attributes.m_shape, width );
} // physical_item_state::set_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the height of the object.
 * \param height The height of the object.
 */
void bear::universe::physical_item_state::set_height( size_type height )
{
  if (!m_fixed && (m_attributes.m_y_fixed == 0))
    shape_traits<shape>::set_height( m_attributes.m_shape, height );
} // physical_item_state::set_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the shape of the object.
 * \param s The new shape.
 */
void bear::universe::physical_item_state::set_shape( const shape& s )
{
  const rectangle_type bounding_box( get_bounding_box() );

  m_attributes.m_shape = s;

  if ( m_fixed || (m_attributes.m_x_fixed != 0) )
    {
      shape_traits<shape>::set_left
        ( m_attributes.m_shape, bounding_box.left() );
      shape_traits<shape>::set_width
        ( m_attributes.m_shape, bounding_box.width() );
    }

  if ( m_fixed || (m_attributes.m_y_fixed != 0) )
    {
      shape_traits<shape>::set_bottom
        ( m_attributes.m_shape, bounding_box.bottom() );
      shape_traits<shape>::set_height
        ( m_attributes.m_shape, bounding_box.height() );
    }
} // physical_item_state::set_shape()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the shape of the object.
 */
bear::universe::shape bear::universe::physical_item_state::get_shape() const
{
  return m_attributes.m_shape;
} // physical_item_state::get_shape()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the mass of the object.
 * \param m The new mass.
 */
void bear::universe::physical_item_state::set_mass( double m )
{
  m_attributes.m_mass = m;
} // physical_item_state::set_mass()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the density of the object.
 * \param d The new density.
 */
void bear::universe::physical_item_state::set_density( double d )
{
  m_attributes.m_density = d;
} // physical_item_state::set_density()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assigns the properties of another state to this one.
 * \param s The instance from which we copy the properties.
 */
void bear::universe::physical_item_state::set_physical_state
( const physical_item_state& s )
{
  if ( is_fixed() )
    return;

  m_attributes = s.m_attributes;

  m_attributes.m_x_fixed = s.m_attributes.m_x_fixed;
  m_attributes.m_y_fixed = s.m_attributes.m_y_fixed;

  if ( s.is_fixed() )
    fix();
} // physical_item_state::set_physical_state()

/*----------------------------------------------------------------------------*/
/**
 * \brief Give a string representation of the item.
 * \param str (out) The result of the convertion.
 */
void bear::universe::physical_item_state::to_string( std::string& str ) const
{
  std::ostringstream oss;

  oss << "0x" << std::hex << this;
  oss << "\nmass: " << get_mass();
  oss << "\npos: " << get_left() << ' ' << get_bottom();
  oss << "\nsize: " << get_width() << ' ' << get_height();
  oss << "\nspeed: " << get_speed().x << ' ' << get_speed().y;
  oss << "\naccel: " << get_acceleration().x << ' ' << get_acceleration().y;
  oss << "\nangular speed: " << get_angular_speed();
  oss << "\nforce (int.): " << get_internal_force().x << ' '
      << get_internal_force().y;
  oss << "\nforce (ext.): " << get_external_force().x << ' '
      << get_external_force().y;
  oss << "\nfriction: s=" << get_friction();
  oss << " c=" << get_contact_friction();
  oss << "\ndensity: " << get_density();
  oss << "\nangle: " << get_system_angle();
  oss << "\nfixed: " << is_fixed() << ' ' << m_attributes.m_x_fixed << ' '
      << m_attributes.m_y_fixed;
  oss << "\nphantom/c.m.i./art./weak.: " << is_phantom() << ' '
      << can_move_items() << ' ' << is_artificial() << ' '
      << has_weak_collisions();
  oss << "\ncontact: { ";

  if ( has_left_contact() )
    oss << "left [" << get_left_contact().get_min() << ", "
        << get_left_contact().get_max() << "] ";
  if ( has_right_contact() )
    oss << "right [" << get_right_contact().get_min() << ", "
        << get_right_contact().get_max() << "] ";
  if ( has_top_contact() )
    oss << "top [" << get_top_contact().get_min() << ", "
        << get_top_contact().get_max() << "] ";
  if ( has_bottom_contact() )
    oss << "bottom [" << get_bottom_contact().get_min() << ", "
        << get_bottom_contact().get_max() << "] ";
  if ( has_middle_contact() )
    oss << "middle ";

  oss << "}";

  str += oss.str();
} // physical_item_state::to_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output a text representation of an item.
 * \param os The stream in which we write.
 * \param item The item to output.
 */
std::ostream& std::operator<<
( std::ostream& os, const bear::universe::physical_item_state& item )
{
  std::string s;
  item.to_string(s);
  return os << s;
} // operator<<()
