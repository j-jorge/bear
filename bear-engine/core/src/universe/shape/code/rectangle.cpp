/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::rectangle class.
 * \author Julien Jorge
 */
#include "universe/shape/rectangle.hpp"

#include "universe/shape/curved_box.hpp"
#include "universe/shape/shape_traits.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::rectangle::rectangle()
  : m_bottom_left(0, 0), m_size(0, 0)
{

} // rectangle::rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor from a universe::rectangle_type.
 */
bear::universe::rectangle::rectangle( const rectangle_type& that )
  : m_bottom_left( that.bottom_left() ), m_size( that.size() )
{

} // rectangle::rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instantiates a copy of this instance.
 */
bear::universe::rectangle* bear::universe::rectangle::clone() const
{
  return new rectangle( *this );
} // rectangle::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this rectangle intersects another given rectangle.
 * \param that The other rectangle.
 */
bool bear::universe::rectangle::intersects( const rectangle& that ) const
{
  return bounding_box_intersects( that );
} // rectangle::intersects()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this rectangle intersects a curved_box.
 * \param that The curved_box.
 */
bool bear::universe::rectangle::intersects( const curved_box& that ) const
{
  return that.intersects( *this );
} // rectangle::intersects()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the coordinate of the bottom edge.
 */
bear::universe::coordinate_type bear::universe::rectangle::do_get_bottom() const
{
  return m_bottom_left.y;
} // rectangle::do_get_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom edge is at a given position.
 * \param p The position.
 */
void bear::universe::rectangle::do_set_bottom( coordinate_type p )
{
  m_bottom_left.y = p;
} // rectangle::do_set_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the coordinate of the left edge.
 */
bear::universe::coordinate_type bear::universe::rectangle::do_get_left() const
{
  return m_bottom_left.x;
} // rectangle::do_get_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its left edge is at a given position.
 * \param p The position.
 */
void bear::universe::rectangle::do_set_left( coordinate_type p )
{
  m_bottom_left.x = p;
} // rectangle::do_set_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the width of the shape.
 */
bear::universe::size_type bear::universe::rectangle::do_get_width() const
{
  return m_size.x;
} // rectangle::do_get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Resize the shape to a given width without changing its bottom left
 *        position.
 * \param s The new width.
 */
void bear::universe::rectangle::do_set_width( size_type s )
{
  m_size.x = s;
} // rectangle::do_set_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the height of the shape.
 */
bear::universe::size_type bear::universe::rectangle::do_get_height() const
{
  return m_size.y;
} // rectangle::do_get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Resize the shape to a given height without changing its bottom left
 *        position.
 * \param s The new height.
 */
void bear::universe::rectangle::do_set_height( size_type s )
{
  m_size.y = s;
} // rectangle::do_set_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this shape intersects another shape.
 */
bool bear::universe::rectangle::do_intersects( const shape_base& that ) const
{
  return that.intersects( *this );
} // rectangle::do_intersects()
