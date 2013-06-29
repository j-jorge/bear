/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::curved_box class.
 * \author Julien Jorge
 */
#include "universe/shape/curved_box.hpp"

#include "universe/shape/rectangle.hpp"
#include "universe/shape/shape_traits.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::curved_box::curved_box()
  : m_bottom_left(0, 0), m_size(0, 0), m_steepness(0), m_top_margin(0),
    m_left_control_point(0, 0), m_right_control_point(0, 0), m_line_width(10)
{

} // curved_box::curved_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instantiates a copy of this instance.
 */
bear::universe::curved_box* bear::universe::curved_box::clone() const
{
  return new curved_box( *this );
} // curved_box::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this curved_box intersects a given rectangle.
 * \param that The other curved_box.
 */
bool bear::universe::curved_box::intersects( const rectangle& that ) const
{
  if ( shape_traits<rectangle>::get_bottom( that )
       > shape_traits<curved_box>::get_bottom( *this ) )
    return check_intersection_above
      ( shape_traits<rectangle>::get_bottom_left( that ),
        shape_traits<rectangle>::get_bottom_right( that ) );

  return bounding_box_intersects( that );
} // curved_box::intersects()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this curved_box intersects another given curved_box.
 * \param that The curved_box.
 */
bool bear::universe::curved_box::intersects( const curved_box& that ) const
{
  if ( shape_traits<curved_box>::get_bottom( that )
       < shape_traits<curved_box>::get_bottom( *this ) )
    return that.intersects( *this );

  return check_intersection_above
    ( shape_traits<curved_box>::get_bottom_left( that ),
      shape_traits<curved_box>::get_bottom_right( that ) );
} // curved_box::intersects()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this shape intersects another shape.
 * \param that The other shape.
 */
bool bear::universe::curved_box::intersects_strict
( const shape_base& that ) const
{
  curved_box strict_this(*this);
  strict_this.m_line_width = 0;

  return that.intersects( strict_this );
} // curved_box::intersects_strict()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the steepness of the top of the shape.
 */
bear::universe::coordinate_type
bear::universe::curved_box::get_steepness() const
{
  return m_steepness;
} // curved_box::get_steepness()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the steepness of the top of the shape.
 * \param s The new steepness of the shape.
 */
void bear::universe::curved_box::set_steepness( coordinate_type s )
{
  m_steepness = s;
} // curved_box::set_steepness()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the margin above the shape.
 */
bear::universe::coordinate_type bear::universe::curved_box::get_margin() const
{
  return m_top_margin;
} // curved_box::get_margin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the margin above the shape.
 * \param s The new margin.
 */
void bear::universe::curved_box::set_margin( coordinate_type s )
{
  m_top_margin = s;
} // curved_box::set_margin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the control point for the left side of the shape.
 */
bear::universe::vector_type
bear::universe::curved_box::get_left_control_point() const
{
  return m_left_control_point;
} // curved_box::get_left_control_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the control point for the left side of the shape.
 * \param p The new control point.
 */
void bear::universe::curved_box::set_left_control_point( const vector_type& p )
{
  m_left_control_point = p;
} // curved_box::set_left_control_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the control point for the right side of the shape.
 */
bear::universe::vector_type
bear::universe::curved_box::get_right_control_point() const
{
  return m_right_control_point;
} // curved_box::get_right_control_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the control point for the right side of the shape.
 * \param p The new control point.
 */
void bear::universe::curved_box::set_right_control_point( const vector_type& p )
{
  m_right_control_point = p;
} // curved_box::set_right_control_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns a description of the top of the shape.
 */
bear::universe::curved_box::curve_type
bear::universe::curved_box::get_curve() const
{
  position_type top_left_anchor( 0, get_height() - m_top_margin );
  position_type top_right_anchor( get_width(), get_height() - m_top_margin );

  if ( m_steepness > 0 )
    top_left_anchor.y -= m_steepness;
  else
    top_right_anchor.y += m_steepness;

  const position_type left_control_point
    ( m_left_control_point + top_left_anchor );
  const position_type right_control_point
    ( m_right_control_point + top_right_anchor );
  
  curve_type result;

  result.push_back
    ( curve_type::control_point
      ( top_left_anchor, top_left_anchor, left_control_point ) );
  result.push_back
    ( curve_type::control_point
      ( top_right_anchor, right_control_point, top_right_anchor ) );
  
  return result;
} // curved_box::get_curve()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the Y-coordinate on the top according to a given X-coordinate.
 * \param x The considered X-coordinate.
 */
bear::universe::coordinate_type 
bear::universe::curved_box::get_y_at_x( coordinate_type x ) const
{
  coordinate_type result( shape_traits<curved_box>::get_bottom( *this ) );
  
  const curve_type c( get_curve() );
  const std::vector<curve_type::section::resolved_point> p
    ( c.get_section( c.begin() ).get_point_at_x
      ( x - shape_traits<curved_box>::get_left( *this ) ) );

  if ( p.size() > 0 )
    result += p[0].get_position().y;

  return result;
} // curved_box::get_y_at_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the coordinate of the bottom edge.
 */
bear::universe::coordinate_type
bear::universe::curved_box::do_get_bottom() const
{
  return m_bottom_left.y;
} // curved_box::do_get_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom edge is at a given position.
 * \param p The position.
 */
void bear::universe::curved_box::do_set_bottom( coordinate_type p )
{
  m_bottom_left.y = p;
} // curved_box::do_set_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the coordinate of the left edge.
 */
bear::universe::coordinate_type bear::universe::curved_box::do_get_left() const
{
  return m_bottom_left.x;
} // curved_box::do_get_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its left edge is at a given position.
 * \param p The position.
 */
void bear::universe::curved_box::do_set_left( coordinate_type p )
{
  m_bottom_left.x = p;
} // curved_box::do_set_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the width of the shape.
 */
bear::universe::size_type bear::universe::curved_box::do_get_width() const
{
  return m_size.x;
} // curved_box::do_get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Resize the shape to a given width without changing its bottom left
 *        position.
 * \param s The new width.
 */
void bear::universe::curved_box::do_set_width( size_type s )
{
  m_size.x = s;
} // curved_box::do_set_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the height of the shape.
 */
bear::universe::size_type bear::universe::curved_box::do_get_height() const
{
  return m_size.y + m_top_margin;
} // curved_box::do_get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Resize the shape to a given height without changing its bottom left
 *        position.
 * \param s The new height.
 */
void bear::universe::curved_box::do_set_height( size_type s )
{
  m_size.y = std::max( size_type(0), s - m_top_margin );
} // curved_box::do_set_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this shape intersects another shape.
 * \param that The other shape.
 */
bool bear::universe::curved_box::do_intersects( const shape_base& that ) const
{
  return that.intersects( *this );
} // curved_box::do_intersects()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if there is an intersection regarding the bottom edge of another
 *        shape.
 * \param bottom_left_position The bottom left position of the other shape.
 * \param bottom_right_position The bottom right position of the other shape.
 */
bool bear::universe::curved_box::check_intersection_above
( const position_type& bottom_left_position,
  const position_type& bottom_right_position ) const
{
  // Check the horizontal center of the other
  const position_type middle_position
    ( ( bottom_left_position + bottom_right_position ) / 2 );
  
  coordinate_type x( middle_position.x );

  // The horizontal center of the other is on our left. We apply the test as if
  // it was on our left edge.
  if ( middle_position.x < shape_traits<curved_box>::get_left( *this ) )
    x = shape_traits<curved_box>::get_left( *this );

  // The horizontal center of the other is on our left. We apply the test as if
  // it was on our right edge.
  if ( middle_position.x > shape_traits<curved_box>::get_right( *this ) )
    x = shape_traits<curved_box>::get_right( *this );

  // We check if the vertical position of the other is above or below the
  // curve. In the latter case, there is an intersection.
  
  return get_y_at_x( x ) + m_line_width > middle_position.y;
} // curved_box::check_intersection_above()
