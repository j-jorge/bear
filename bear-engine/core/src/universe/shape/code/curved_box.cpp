/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::curved_box class.
 * \author Julien Jorge
 */
#include "universe/shape/curved_box.hpp"

#include "universe/shape/rectangle.hpp"

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
  return false;
} // curved_box::intersects()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this curved_box intersects another given curved_box.
 * \param that The curved_box.
 */
bool bear::universe::curved_box::intersects( const curved_box& that ) const
{
  return false;
} // curved_box::intersects()

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
  return m_size.y;
} // curved_box::do_get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Resize the shape to a given height without changing its bottom left
 *        position.
 * \param s The new height.
 */
void bear::universe::curved_box::do_set_height( size_type s )
{
  m_size.y = s;
} // curved_box::do_set_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this shape intersects another shape.
 */
bool bear::universe::curved_box::do_intersects( const shape_base& that ) const
{
  return that.intersects( *this );
} // curved_box::do_intersects()
