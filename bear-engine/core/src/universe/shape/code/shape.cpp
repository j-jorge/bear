/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::shape class.
 * \author Julien Jorge
 */
#include "universe/shape/shape.hpp"

#include "universe/shape/shape_base.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::shape::shape()
  : m_impl( NULL )
{

} // shape::shape()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param s The underlying shape.
 */
bear::universe::shape::shape( const shape_base& s )
  : m_impl( s.clone() )
{

} // shape::shape()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy.
 */
bear::universe::shape::shape( const shape& that )
  : m_impl( ( that.m_impl == NULL ) ? NULL : that.m_impl->clone() )
{

} // shape::shape()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::universe::shape::~shape()
{
  delete m_impl;
} // shape::shape()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assigns a shape to this one.
 * \param that The instance to copy.
 */
bear::universe::shape& bear::universe::shape::operator=( shape that )
{
  std::swap( m_impl, that.m_impl );

  return *this;
} // shape::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this shape has an intersection with another given shape.
 * \param that The other shape.
 */
bool bear::universe::shape::intersects( const shape& that ) const
{
  if ( (m_impl == NULL) || (that.m_impl == NULL) )
    return false;
  else
    return m_impl->intersects( *that.m_impl );
} // shape::intersects()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the size of the object.
 */
bear::universe::size_box_type bear::universe::shape::get_size() const
{
  return size_box_type( get_width(), get_height() );
} // shape::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size of the object.
 * \param size The new size.
 */
void bear::universe::shape::set_size( const size_box_type& size )
{
  set_width( size.x );
  set_height( size.y );
} // shape::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the width of the shape.
 */
bear::universe::size_type bear::universe::shape::get_width() const
{
  if ( m_impl == NULL )
    return 0;
  else
    return m_impl->get_width();
} // shape::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the width of the shape.
 * \param width The width of the shape.
 */
void bear::universe::shape::set_width( size_type width )
{
  if ( m_impl != NULL )
    m_impl->set_width( width );
} // shape::set_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the height of the shape.
 */
bear::universe::size_type
bear::universe::shape::get_height() const
{
  if ( m_impl == NULL )
    return 0;
  else
    return m_impl->get_height();
} // shape::get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the height of the shape.
 * \param height The height of the shape.
 */
void bear::universe::shape::set_height( size_type height )
{
  if ( m_impl != NULL )
    m_impl->set_height( height );
} // shape::set_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the bouding box of this shape.
 * \param r the new bounding box of the shape.
 */
void bear::universe::shape::set_bounding_box( const rectangle_type& r )
{
  set_bottom_left( r.bottom_left() );
  set_size( r.size() );
} // shape::set_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the bouding box of this shape.
 */
bear::universe::rectangle_type
bear::universe::shape::get_bounding_box() const
{
  return rectangle_type( get_bottom_left(), get_bottom_left() + get_size() );
} // shape::get_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its top is at a given coordinate.
 * \param pos The new position.
 */
void bear::universe::shape::set_top( coordinate_type pos )
{
  set_bottom( pos - get_height() );
} // shape::set_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom is at a given coordinate.
 * \param pos The new position.
 */
void bear::universe::shape::set_bottom( coordinate_type pos )
{
  if ( m_impl != NULL )
    m_impl->set_bottom( pos );
} // shape::set_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its left is at a given coordinate.
 * \param pos The new position.
 */
void bear::universe::shape::set_left( coordinate_type pos )
{
  if ( m_impl != NULL )
    m_impl->set_left( pos );
} // shape::set_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its right is at a given coordinate.
 * \param pos The new position.
 */
void bear::universe::shape::set_right( coordinate_type pos )
{
  set_left( pos - get_width() );
} // shape::set_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its horizontal center is at a given
 *        coordinate.
 * \param pos The new position.
 */
void bear::universe::shape::set_horizontal_middle( coordinate_type pos )
{
  set_left( pos - get_width() / 2 );
} // shape::set_horizontal_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its vertical middle is at a given
 *        coordinate.
 * \param pos The new position.
 */
void bear::universe::shape::set_vertical_middle( coordinate_type pos )
{
  set_bottom( pos - get_height() / 2 );
} // shape::set_vertical_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its top left corner is at a given position.
 * \param pos The new position.
 */
void bear::universe::shape::set_top_left( const position_type& pos )
{
  set_left(pos.x);
  set_top(pos.y);
} // shape::set_top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its top edge is at a given
 *        position.
 * \param pos The new position.
 */
void bear::universe::shape::set_top_middle( const position_type& pos )
{
  set_horizontal_middle(pos.x);
  set_top(pos.y);
} // shape::set_top_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its top right corner is at a given position.
 * \param pos The new position.
 */
void
bear::universe::shape::set_top_right( const position_type& pos )
{
  set_right(pos.x);
  set_top(pos.y);
} // shape::set_top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom left corner is at a given
 *        position.
 * \param pos The new position.
 */
void bear::universe::shape::set_bottom_left( const position_type& pos )
{
  set_left(pos.x);
  set_bottom(pos.y);
} // shape::set_bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its bottom is at a given
 *        position.
 * \param pos The new position.
 */
void bear::universe::shape::set_bottom_middle( const position_type& pos )
{
  set_horizontal_middle(pos.x);
  set_bottom(pos.y);
} // shape::set_bottom_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom right corner is at a given
 *        position.
 * \param pos The new position.
 */
void bear::universe::shape::set_bottom_right( const position_type& pos )
{
  set_right(pos.x);
  set_bottom(pos.y);
} // shape::set_bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its left edge is at a given
 *        position.
 * \param pos The new position.
 */
void bear::universe::shape::set_left_middle( const position_type& pos )
{
  set_left(pos.x);
  set_vertical_middle(pos.y);
} // shape::set_left_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its right edge is at a given
 *        position.
 * \param pos The new position.
 */
void bear::universe::shape::set_right_middle( const position_type& pos )
{
  set_right(pos.x);
  set_vertical_middle(pos.y);
} // shape::set_right_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its top left corner is at a given position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void
bear::universe::shape::set_top_left( coordinate_type x, coordinate_type y )
{
  set_left(x);
  set_top(y);
} // shape::set_top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its top edge is at a given
 *        position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::shape::set_top_middle
( coordinate_type x, coordinate_type y )
{
  set_horizontal_middle(x);
  set_top(y);
} // shape::set_top_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its top right corner is at a given position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::shape::set_top_right
( coordinate_type x, coordinate_type y )
{
  set_right(x);
  set_top(y);
} // shape::set_top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom left corner is at a given
 *        position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::shape::set_bottom_left
( coordinate_type x, coordinate_type y )
{
  set_left(x);
  set_bottom(y);
} // shape::set_bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its bottom is at a given
 *        position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::shape::set_bottom_middle
( coordinate_type x, coordinate_type y )
{
  set_horizontal_middle(x);
  set_bottom(y);
} // shape::set_bottom_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom right corner is at a given
 *        position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::shape::set_bottom_right
( coordinate_type x, coordinate_type y )
{
  set_right(x);
  set_bottom(y);
} // shape::set_bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its left edge is at a given
 *        position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::shape::set_left_middle
( coordinate_type x, coordinate_type y )
{
  set_left(x);
  set_vertical_middle(y);
} // shape::set_left_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its right edge is at a given
 *        position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::shape::set_right_middle
( coordinate_type x, coordinate_type y )
{
  set_right(x);
  set_vertical_middle(y);
} // shape::set_right_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the lowest X-coordinate covered by the bounding box of this
 *        shape.
 */
bear::universe::coordinate_type bear::universe::shape::get_left() const
{
  if ( m_impl == NULL )
    return 0;
  else
    return m_impl->get_left();
} // shape::get_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the lowest Y-coordinate covered by the bounding box of this
 *        shape.
 */
bear::universe::coordinate_type bear::universe::shape::get_top() const
{
  return get_bottom() + get_height();
} // shape::get_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the highest X-coordinate covered by the bounding box of this
 *        shape.
 */
bear::universe::coordinate_type bear::universe::shape::get_right() const
{
  return get_left() + get_width();
} // shape::get_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the highest Y-coordinate covered by the bounding box of this
 *        shape.
 */
bear::universe::coordinate_type bear::universe::shape::get_bottom() const
{
  if ( m_impl == NULL )
    return 0;
  else
    return m_impl->get_bottom();
} // shape::get_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the x-coordinate of the center of this shape.
 */
bear::universe::coordinate_type
bear::universe::shape::get_horizontal_middle() const
{
  return get_left() + get_width() / 2;
} // shape::get_horizontal_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the Y-coordinate of the center of this shape.
 */
bear::universe::coordinate_type
bear::universe::shape::get_vertical_middle() const
{
  return get_bottom() + get_height() / 2;
} // shape::get_vertical_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the top left corner of the shape.
 */
bear::universe::position_type
bear::universe::shape::get_top_left() const
{
  return position_type( get_left(), get_top() );
} // shape::get_top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the middle of the top edge of the shape.
 */
bear::universe::position_type
 bear::universe::shape::get_top_middle() const
{
  return position_type( get_center_of_mass().x, get_top() );
} // shape::get_top_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the top right corner of the shape.
 */
bear::universe::position_type
bear::universe::shape::get_top_right() const
{
  return position_type( get_right(), get_top() );
} // shape::get_top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the bottom left corner of the shape.
 */
bear::universe::position_type
bear::universe::shape::get_bottom_left() const
{
  return position_type( get_left(), get_bottom() );
} // shape::get_bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the middle of the bottom edge of the shape.
 */
bear::universe::position_type
bear::universe::shape::get_bottom_middle() const
{
  return position_type( get_center_of_mass().x, get_bottom() );
} // shape::get_bottom_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the bottom right corner of the shape.
 */
bear::universe::position_type
bear::universe::shape::get_bottom_right() const
{
  return position_type( get_right(), get_bottom() );
} // shape::get_bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the middle of the left edge of the shape.
 */
bear::universe::position_type
bear::universe::shape::get_left_middle() const
{
  return position_type( get_left(), get_center_of_mass().y );
} // shape::get_left_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the middle of the right edge of the shape.
 */
bear::universe::position_type
bear::universe::shape::get_right_middle() const
{
  return position_type( get_right(), get_center_of_mass().y );
} // shape::get_right_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position, in the world, of the center of mass.
 */
bear::universe::position_type
bear::universe::shape::get_center_of_mass() const
{
  return get_bottom_left() + get_size() / 2;
} // shape::get_center_of_mass()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its center is at a given position.
 * \param pos The new center of mass.
 */
void bear::universe::shape::set_center_of_mass( const position_type& pos )
{
  set_bottom_left( pos - get_size() / 2 );
} // shape::set_center_of_mass()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its center is at a given position.
 * \param x The new x_coordinate of the center of mass.
 * \param y The new y-coordinate of the center of mass.
 */
void bear::universe::shape::set_center_of_mass
( coordinate_type x, coordinate_type y )
{
  set_center_of_mass( position_type(x, y) );
} // shape::set_center_of_mass()
