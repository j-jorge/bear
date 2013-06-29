/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::model_mark_placement class.
 * \author Julien Jorge
 */
#include "engine/model/model_mark_placement.hpp"

#include <claw/tween/easing/easing_none.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::model_mark_placement::model_mark_placement()
: m_angle_easing(&claw::tween::easing_none::ease_in_out),
  m_x_easing(&claw::tween::easing_none::ease_in_out),
  m_y_easing(&claw::tween::easing_none::ease_in_out),
  m_width_easing(&claw::tween::easing_none::ease_in_out),
  m_height_easing(&claw::tween::easing_none::ease_in_out)
{

} // model_mark_placement::model_mark_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param id The identifier of the mark.
 * \param x The x-position.
 * \param y The y-position.
 * \param w The width of the box around the mark.
 * \param h The height of the box around the mark.
 * \param depth The depth-position.
 * \param angle The angle.
 * \param visible The visibility.
 * \param collision_function The name of the function called when a collision
 *        occurs on the box around the mark.
 */
bear::engine::model_mark_placement::model_mark_placement
( unsigned int id, universe::coordinate_type x, universe::coordinate_type y,
  universe::coordinate_type w, universe::coordinate_type h, int depth,
  double angle, bool visible, const std::string& collision_function )
  : m_mark_id(id), m_position(x, y), m_size(w, h), m_depth(depth),
    m_angle(angle), m_visible(visible),
    m_collision_function(collision_function),
    m_angle_easing(&claw::tween::easing_none::ease_in_out),
    m_x_easing(&claw::tween::easing_none::ease_in_out),
    m_y_easing(&claw::tween::easing_none::ease_in_out),
    m_width_easing(&claw::tween::easing_none::ease_in_out),
    m_height_easing(&claw::tween::easing_none::ease_in_out)
{

} // model_mark_placement::model_mark_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the identifier or the mark concerned by this placement.
 * \param i The identifier of the mark.
 */
void bear::engine::model_mark_placement::set_mark_id( unsigned int i )
{
  m_mark_id = i;
} // model_mark_placement::set_mark_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the identifier or the mark concerned by this placement.
 */
unsigned int bear::engine::model_mark_placement::get_mark_id() const
{
  return m_mark_id;
} // model_mark_placement::get_mark_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of the mark.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::engine::model_mark_placement::set_position
( universe::coordinate_type x, universe::coordinate_type y )
{
  m_position.set(x, y);
} // model_mark_placement::set_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of the mark.
 * \param pos The new position.
 */
void bear::engine::model_mark_placement::set_position
( const universe::position_type& pos )
{
  m_position = pos;
} // model_mark_placement::set_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the mark.
 */
const bear::universe::position_type&
bear::engine::model_mark_placement::get_position() const
{
  return m_position;
} // model_mark_placement::get_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the x-position of the mark.
 * \param x The new position.
 */
void bear::engine::model_mark_placement::set_x_position
( universe::coordinate_type x )
{
  m_position.x = x;
} // model_mark_placement::set_x_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the y-position of the mark.
 * \param y The new position.
 */
void bear::engine::model_mark_placement::set_y_position
( universe::coordinate_type y )
{
  m_position.y = y;
} // model_mark_placement::set_y_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size of the box around the mark.
 * \param s The new size.
 */
void bear::engine::model_mark_placement::set_size
( const universe::size_box_type& s )
{
  m_size = s;
} // model_mark_placement::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the box around the mark.
 */
const bear::universe::size_box_type&
bear::engine::model_mark_placement::get_size() const
{
  return m_size;
} // model_mark_placement::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the width of the mark.
 * \param s The new width.
 */
void bear::engine::model_mark_placement::set_width( universe::size_type s )
{
  m_size.x = s;
} // model_mark_placement::set_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the height of the mark.
 * \param s The new height.
 */
void bear::engine::model_mark_placement::set_height( universe::size_type s )
{
  m_size.y = s;
} // model_mark_placement::set_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the depth-position of the mark.
 * \param z The new depth-position.
 */
void bear::engine::model_mark_placement::set_depth_position( int z )
{
  m_depth = z;
} // model_mark_placement::set_depth_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the depth-position of the mark.
 */
int bear::engine::model_mark_placement::get_depth_position() const
{
  return m_depth;
} // model_mark_placement::get_depth_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the angle of the mark.
 * \param a The new angle.
 */
void bear::engine::model_mark_placement::set_angle( double a )
{
  m_angle = a;
} // model_mark_placement::set_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the angle of the mark.
 */
double bear::engine::model_mark_placement::get_angle() const
{
  return m_angle;
} // model_mark_placement::get_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set if the mark is visible or not.
 * \param v Visible or not.
 */
void bear::engine::model_mark_placement::set_visibility( bool v )
{
  m_visible = v;
} // model_mark_placement::set_visibility()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the mark is visible or not.
 */
bool bear::engine::model_mark_placement::is_visible() const
{
  return m_visible;
} // model_mark_placement::is_visible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the function to call when something collides in the
 *        box around the mark.
 * \param f The name of the function.
 */
void bear::engine::model_mark_placement::set_collision_function
( const std::string& f )
{
  m_collision_function = f;
} // model_mark_placement::set_collision_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the function to call when something collides in the
 *        box around the mark.
 */
const std::string&
bear::engine::model_mark_placement::get_collision_function() const
{
  return m_collision_function;
} // model_mark_placement::get_collision_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the easing function for the angle.
 */
bear::engine::model_mark_placement::easing_function
bear::engine::model_mark_placement::get_angle_easing() const
{
  return m_angle_easing;
} // model_mark_placement::get_angle_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the easing function for the angle.
 * \param f The function.
 */
void bear::engine::model_mark_placement::set_angle_easing( easing_function f )
{
  m_angle_easing = f;
} // model_mark_placement::set_angle_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the easing function for the x-position.
 */
bear::engine::model_mark_placement::easing_function
bear::engine::model_mark_placement::get_x_position_easing() const
{
  return m_x_easing;
} // model_mark_placement::get_x_position_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the easing function for the x-position.
 * \param f The function.
 */
void bear::engine::model_mark_placement::set_x_position_easing
( easing_function f )
{
  m_x_easing = f;
} // model_mark_placement::()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the easing function for the y-position.
 */
bear::engine::model_mark_placement::easing_function
bear::engine::model_mark_placement::get_y_position_easing() const
{
  return m_y_easing;
} // model_mark_placement::get_y_position_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the easing function for the y-position.
 * \param f The function.
 */
void bear::engine::model_mark_placement::set_y_position_easing
( easing_function f )
{
  m_y_easing = f;
} // model_mark_placement::set_y_position_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the easing function for the width.
 */
bear::engine::model_mark_placement::easing_function
bear::engine::model_mark_placement::get_width_easing() const
{
  return m_width_easing;
} // model_mark_placement::get_width_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the easing function for the width.
 * \param f The function.
 */
void bear::engine::model_mark_placement::set_width_easing( easing_function f )
{
  m_width_easing = f;
} // model_mark_placement::set_width_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the easing function for the height.
 */
bear::engine::model_mark_placement::easing_function
bear::engine::model_mark_placement::get_height_easing() const
{
  return m_height_easing;
} // model_mark_placement::get_height_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the easing function for the height.
 * \param f The function.
 */
void bear::engine::model_mark_placement::set_height_easing( easing_function f )
{
  m_height_easing = f;
} // model_mark_placement::set_height_easing()
