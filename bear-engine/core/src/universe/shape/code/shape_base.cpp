/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::shape_base class.
 * \author Julien Jorge
 */
#include "universe/shape/shape_base.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::universe::shape_base::~shape_base()
{
  // nothing to do
} // shape_base::shape_base()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this shape has an intersection with another given shape.
 * \param that The other shape.
 */
bool bear::universe::shape_base::intersects( const shape_base& that ) const
{
  return do_intersects( that );
} // shape_base::intersects()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the size of the object.
 */
bear::universe::size_box_type bear::universe::shape_base::get_size() const
{
  return size_box_type( get_width(), get_height() );
} // shape_base::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size of the object.
 * \param size The new size.
 */
void bear::universe::shape_base::set_size( const size_box_type& size )
{
  set_width( size.x );
  set_height( size.y );
} // shape_base::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the width of the shape.
 */
bear::universe::size_type bear::universe::shape_base::get_width() const
{
  return do_get_width();
} // shape_base::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the width of the shape.
 * \param width The width of the shape.
 */
void bear::universe::shape_base::set_width( size_type width )
{
  do_set_width( width );
} // shape_base::set_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the height of the shape.
 */
bear::universe::size_type
bear::universe::shape_base::get_height() const
{
  return do_get_height();
} // shape_base::get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the height of the shape.
 * \param height The height of the shape.
 */
void bear::universe::shape_base::set_height( size_type height )
{
  do_set_height( height );
} // shape_base::set_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the bouding box of this shape.
 * \param r the new bounding box of the shape.
 */
void bear::universe::shape_base::set_bounding_box( const rectangle_type& r )
{
  set_bottom_left( r.bottom_left() );
  set_size( r.size() );
} // shape_base::set_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the bouding box of this shape.
 */
bear::universe::rectangle_type
bear::universe::shape_base::get_bounding_box() const
{
  return rectangle_type( get_bottom_left(), get_bottom_left() + get_size() );
} // shape_base::get_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its top is at a given coordinate.
 * \param pos The new position.
 */
void bear::universe::shape_base::set_top( coordinate_type pos )
{
  set_bottom( pos - get_height() );
} // shape_base::set_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom is at a given coordinate.
 * \param pos The new position.
 */
void bear::universe::shape_base::set_bottom( coordinate_type pos )
{
  do_set_bottom( pos );
} // shape_base::set_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its left is at a given coordinate.
 * \param pos The new position.
 */
void bear::universe::shape_base::set_left( coordinate_type pos )
{
  do_set_left( pos );
} // shape_base::set_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its right is at a given coordinate.
 * \param pos The new position.
 */
void bear::universe::shape_base::set_right( coordinate_type pos )
{
  set_left( pos - get_width() );
} // shape_base::set_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its horizontal center is at a given
 *        coordinate.
 * \param pos The new position.
 */
void bear::universe::shape_base::set_horizontal_middle( coordinate_type pos )
{
  set_left( pos - get_width() / 2 );
} // shape_base::set_horizontal_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its vertical middle is at a given
 *        coordinate.
 * \param pos The new position.
 */
void bear::universe::shape_base::set_vertical_middle( coordinate_type pos )
{
  set_bottom( pos - get_height() / 2 );
} // shape_base::set_vertical_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its top left corner is at a given position.
 * \param pos The new position.
 */
void bear::universe::shape_base::set_top_left( const position_type& pos )
{
  set_left(pos.x);
  set_top(pos.y);
} // shape_base::set_top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its top edge is at a given
 *        position.
 * \param pos The new position.
 */
void bear::universe::shape_base::set_top_middle( const position_type& pos )
{
  set_horizontal_middle(pos.x);
  set_top(pos.y);
} // shape_base::set_top_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its top right corner is at a given position.
 * \param pos The new position.
 */
void
bear::universe::shape_base::set_top_right( const position_type& pos )
{
  set_right(pos.x);
  set_top(pos.y);
} // shape_base::set_top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom left corner is at a given
 *        position.
 * \param pos The new position.
 */
void bear::universe::shape_base::set_bottom_left( const position_type& pos )
{
  set_left(pos.x);
  set_bottom(pos.y);
} // shape_base::set_bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its bottom is at a given
 *        position.
 * \param pos The new position.
 */
void bear::universe::shape_base::set_bottom_middle( const position_type& pos )
{
  set_horizontal_middle(pos.x);
  set_bottom(pos.y);
} // shape_base::set_bottom_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom right corner is at a given
 *        position.
 * \param pos The new position.
 */
void bear::universe::shape_base::set_bottom_right( const position_type& pos )
{
  set_right(pos.x);
  set_bottom(pos.y);
} // shape_base::set_bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its left edge is at a given
 *        position.
 * \param pos The new position.
 */
void bear::universe::shape_base::set_left_middle( const position_type& pos )
{
  set_left(pos.x);
  set_vertical_middle(pos.y);
} // shape_base::set_left_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its right edge is at a given
 *        position.
 * \param pos The new position.
 */
void bear::universe::shape_base::set_right_middle( const position_type& pos )
{
  set_right(pos.x);
  set_vertical_middle(pos.y);
} // shape_base::set_right_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its top left corner is at a given position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void
bear::universe::shape_base::set_top_left( coordinate_type x, coordinate_type y )
{
  set_left(x);
  set_top(y);
} // shape_base::set_top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its top edge is at a given
 *        position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::shape_base::set_top_middle
( coordinate_type x, coordinate_type y )
{
  set_horizontal_middle(x);
  set_top(y);
} // shape_base::set_top_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its top right corner is at a given position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::shape_base::set_top_right
( coordinate_type x, coordinate_type y )
{
  set_right(x);
  set_top(y);
} // shape_base::set_top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom left corner is at a given
 *        position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::shape_base::set_bottom_left
( coordinate_type x, coordinate_type y )
{
  set_left(x);
  set_bottom(y);
} // shape_base::set_bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its bottom is at a given
 *        position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::shape_base::set_bottom_middle
( coordinate_type x, coordinate_type y )
{
  set_horizontal_middle(x);
  set_bottom(y);
} // shape_base::set_bottom_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom right corner is at a given
 *        position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::shape_base::set_bottom_right
( coordinate_type x, coordinate_type y )
{
  set_right(x);
  set_bottom(y);
} // shape_base::set_bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its left edge is at a given
 *        position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::shape_base::set_left_middle
( coordinate_type x, coordinate_type y )
{
  set_left(x);
  set_vertical_middle(y);
} // shape_base::set_left_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its right edge is at a given
 *        position.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::universe::shape_base::set_right_middle
( coordinate_type x, coordinate_type y )
{
  set_right(x);
  set_vertical_middle(y);
} // shape_base::set_right_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the lowest X-coordinate covered by the bounding box of this
 *        shape.
 */
bear::universe::coordinate_type bear::universe::shape_base::get_left() const
{
  return do_get_left();
} // shape_base::get_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the lowest Y-coordinate covered by the bounding box of this
 *        shape.
 */
bear::universe::coordinate_type bear::universe::shape_base::get_top() const
{
  return get_bottom() + get_height();
} // shape_base::get_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the highest X-coordinate covered by the bounding box of this
 *        shape.
 */
bear::universe::coordinate_type bear::universe::shape_base::get_right() const
{
  return get_left() + get_width();
} // shape_base::get_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the highest Y-coordinate covered by the bounding box of this
 *        shape.
 */
bear::universe::coordinate_type bear::universe::shape_base::get_bottom() const
{
  return do_get_bottom();
} // shape_base::get_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the x-coordinate of the center of this shape.
 */
bear::universe::coordinate_type
bear::universe::shape_base::get_horizontal_middle() const
{
  return get_left() + get_width() / 2;
} // shape_base::get_horizontal_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the Y-coordinate of the center of this shape.
 */
bear::universe::coordinate_type
bear::universe::shape_base::get_vertical_middle() const
{
  return get_bottom() + get_height() / 2;
} // shape_base::get_vertical_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the top left corner of the shape.
 */
bear::universe::position_type
bear::universe::shape_base::get_top_left() const
{
  return position_type( get_left(), get_top() );
} // shape_base::get_top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the middle of the top edge of the shape.
 */
bear::universe::position_type
 bear::universe::shape_base::get_top_middle() const
{
  return position_type( get_center_of_mass().x, get_top() );
} // shape_base::get_top_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the top right corner of the shape.
 */
bear::universe::position_type
bear::universe::shape_base::get_top_right() const
{
  return position_type( get_right(), get_top() );
} // shape_base::get_top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the bottom left corner of the shape.
 */
bear::universe::position_type
bear::universe::shape_base::get_bottom_left() const
{
  return position_type( get_left(), get_bottom() );
} // shape_base::get_bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the middle of the bottom edge of the shape.
 */
bear::universe::position_type
bear::universe::shape_base::get_bottom_middle() const
{
  return position_type( get_center_of_mass().x, get_bottom() );
} // shape_base::get_bottom_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the bottom right corner of the shape.
 */
bear::universe::position_type
bear::universe::shape_base::get_bottom_right() const
{
  return position_type( get_right(), get_bottom() );
} // shape_base::get_bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the middle of the left edge of the shape.
 */
bear::universe::position_type
bear::universe::shape_base::get_left_middle() const
{
  return position_type( get_left(), get_center_of_mass().y );
} // shape_base::get_left_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the middle of the right edge of the shape.
 */
bear::universe::position_type
bear::universe::shape_base::get_right_middle() const
{
  return position_type( get_right(), get_center_of_mass().y );
} // shape_base::get_right_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position, in the world, of the center of mass.
 */
bear::universe::position_type
bear::universe::shape_base::get_center_of_mass() const
{
  return get_bottom_left() + get_size() / 2;
} // shape_base::get_center_of_mass()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its center is at a given position.
 * \param pos The new center of mass.
 */
void bear::universe::shape_base::set_center_of_mass( const position_type& pos )
{
  set_bottom_left( pos - get_size() / 2 );
} // shape_base::set_center_of_mass()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its center is at a given position.
 * \param x The new x_coordinate of the center of mass.
 * \param y The new y-coordinate of the center of mass.
 */
void bear::universe::shape_base::set_center_of_mass
( coordinate_type x, coordinate_type y )
{
  set_center_of_mass( position_type(x, y) );
} // shape_base::set_center_of_mass()
