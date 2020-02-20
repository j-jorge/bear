/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::shape_traits class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the size of the shape.
 * \param s The shape.
 */
template<typename T>
bear::universe::size_box_type
bear::universe::shape_traits<T>::get_size( const shape_type& s )
{
  return size_box_type( get_width(s), get_height(s) );
} // shape_traits::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size of the object.
 * \param s The shape.
 * \param size The new size.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_size
( shape_type& s, const size_box_type& size )
{
  set_width( s, size.x );
  set_height( s, size.y );
} // shape_traits::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the width of the shape.
 * \param s The shape.
 */
template<typename T>
bear::universe::size_type
bear::universe::shape_traits<T>::get_width( const shape_type& s )
{
  return s.get_width();
} // shape_traits::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the width of the shape.
 * \param s The shape.
 * \param width The width of the shape.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_width
( shape_type& s, size_type width )
{
  s.set_width( width );
} // shape_traits::set_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the height of the shape.
 * \param s The shape.
 */
template<typename T>
bear::universe::size_type
bear::universe::shape_traits<T>::get_height( const shape_type& s )
{
  return s.get_height();
} // shape_traits::get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the height of the shape.
 * \param s The shape.
 * \param height The height of the shape.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_height
( shape_type& s, size_type height )
{
  s.set_height( height );
} // shape_traits::set_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the bouding box of this shape_traits.
 * \param s The shape.
 * \param r the new bounding box of the shape.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_bounding_box
( shape_type& s, const rectangle_type& r )
{
  set_bottom_left( s, r.bottom_left() );
  set_size( s, r.size() );
} // shape_traits::set_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the bouding box of this shape_traits.
 * \param s The shape.
 */
template<typename T>
bear::universe::rectangle_type
bear::universe::shape_traits<T>::get_bounding_box( const shape_type& s )
{
  return rectangle_type( get_bottom_left(s), get_bottom_left(s) + get_size(s) );
} // shape_traits::get_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its top is at a given coordinate.
 * \param s The shape.
 * \param pos The new position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_top
( shape_type& s, coordinate_type pos )
{
  set_bottom( s, pos - get_height(s) );
} // shape_traits::set_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom is at a given coordinate.
 * \param s The shape.
 * \param pos The new position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_bottom
( shape_type& s, coordinate_type pos )
{
  s.set_bottom( pos );
} // shape_traits::set_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its left is at a given coordinate.
 * \param s The shape.
 * \param pos The new position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_left
( shape_type& s, coordinate_type pos )
{
  s.set_left( pos );
} // shape_traits::set_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its right is at a given coordinate.
 * \param s The shape.
 * \param pos The new position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_right
( shape_type& s, coordinate_type pos )
{
  set_left( s, pos - get_width(s) );
} // shape_traits::set_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its horizontal center is at a given
 *        coordinate.
 * \param s The shape.
 * \param pos The new position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_horizontal_middle
( shape_type& s, coordinate_type pos )
{
  set_left( s, pos - get_width(s) / 2 );
} // shape_traits::set_horizontal_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its vertical middle is at a given
 *        coordinate.
 * \param s The shape.
 * \param pos The new position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_vertical_middle
( shape_type& s, coordinate_type pos )
{
  set_bottom( s, pos - get_height(s) / 2 );
} // shape_traits::set_vertical_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its top left corner is at a given position.
 * \param s The shape.
 * \param pos The new position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_top_left
( shape_type& s, const position_type& pos )
{
  set_left( s, pos.x );
  set_top( s, pos.y );
} // shape_traits::set_top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its top edge is at a given
 *        position.
 * \param s The shape.
 * \param pos The new position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_top_middle
( shape_type& s, const position_type& pos )
{
  set_horizontal_middle( s, pos.x );
  set_top( s, pos.y );
} // shape_traits::set_top_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its top right corner is at a given position.
 * \param s The shape.
 * \param pos The new position.
 */
template<typename T>
void
bear::universe::shape_traits<T>::set_top_right
( shape_type& s, const position_type& pos )
{
  set_right( s, pos.x );
  set_top( s, pos.y );
} // shape_traits::set_top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom left corner is at a given
 *        position.
 * \param s The shape.
 * \param pos The new position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_bottom_left
( shape_type& s, const position_type& pos )
{
  set_left( s, pos.x );
  set_bottom( s, pos.y );
} // shape_traits::set_bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its bottom is at a given
 *        position.
 * \param s The shape.
 * \param pos The new position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_bottom_middle
( shape_type& s, const position_type& pos )
{
  set_horizontal_middle( s, pos.x );
  set_bottom( s, pos.y );
} // shape_traits::set_bottom_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom right corner is at a given
 *        position.
 * \param s The shape.
 * \param pos The new position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_bottom_right
( shape_type& s, const position_type& pos )
{
  set_right( s, pos.x );
  set_bottom( s, pos.y );
} // shape_traits::set_bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its left edge is at a given
 *        position.
 * \param s The shape.
 * \param pos The new position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_left_middle
( shape_type& s, const position_type& pos )
{
  set_left( s, pos.x );
  set_vertical_middle( s, pos.y );
} // shape_traits::set_left_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its right edge is at a given
 *        position.
 * \param s The shape.
 * \param pos The new position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_right_middle
( shape_type& s, const position_type& pos )
{
  set_right( s, pos.x );
  set_vertical_middle( s, pos.y );
} // shape_traits::set_right_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its top left corner is at a given position.
 * \param s The shape.
 * \param x The new x-position.
 * \param y The new y-position.
 */
template<typename T>
void
bear::universe::shape_traits<T>::set_top_left
( shape_type& s, coordinate_type x, coordinate_type y )
{
  set_left( s, x );
  set_top( s, y );
} // shape_traits::set_top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its top edge is at a given
 *        position.
 * \param s The shape.
 * \param x The new x-position.
 * \param y The new y-position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_top_middle
( shape_type& s, coordinate_type x, coordinate_type y )
{
  set_horizontal_middle(s, x);
  set_top(s, y);
} // shape_traits::set_top_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its top right corner is at a given position.
 * \param s The shape.
 * \param x The new x-position.
 * \param y The new y-position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_top_right
( shape_type& s, coordinate_type x, coordinate_type y )
{
  set_right(s, x);
  set_top(s, y);
} // shape_traits::set_top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom left corner is at a given
 *        position.
 * \param s The shape.
 * \param x The new x-position.
 * \param y The new y-position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_bottom_left
( shape_type& s, coordinate_type x, coordinate_type y )
{
  set_left(s, x);
  set_bottom(s, y);
} // shape_traits::set_bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its bottom is at a given
 *        position.
 * \param s The shape.
 * \param x The new x-position.
 * \param y The new y-position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_bottom_middle
( shape_type& s, coordinate_type x, coordinate_type y )
{
  set_horizontal_middle(s, x);
  set_bottom(s, y);
} // shape_traits::set_bottom_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom right corner is at a given
 *        position.
 * \param s The shape.
 * \param x The new x-position.
 * \param y The new y-position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_bottom_right
( shape_type& s, coordinate_type x, coordinate_type y )
{
  set_right(s, x);
  set_bottom(s, y);
} // shape_traits::set_bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its left edge is at a given
 *        position.
 * \param s The shape.
 * \param x The new x-position.
 * \param y The new y-position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_left_middle
( shape_type& s, coordinate_type x, coordinate_type y )
{
  set_left(s, x);
  set_vertical_middle(s, y);
} // shape_traits::set_left_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that the middle of its right edge is at a given
 *        position.
 * \param s The shape.
 * \param x The new x-position.
 * \param y The new y-position.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_right_middle
( shape_type& s, coordinate_type x, coordinate_type y )
{
  set_right(s, x);
  set_vertical_middle(s, y);
} // shape_traits::set_right_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the lowest X-coordinate covered by the bounding box of this
 *        shape_traits.
 * \param s The shape.
 */
template<typename T>
bear::universe::coordinate_type
bear::universe::shape_traits<T>::get_left( const shape_type& s )
{
  return s.get_left();
} // shape_traits::get_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the lowest Y-coordinate covered by the bounding box of this
 *        shape_traits.
 * \param s The shape.
 */
template<typename T>
bear::universe::coordinate_type
bear::universe::shape_traits<T>::get_top( const shape_type& s )
{
  return get_bottom(s) + get_height(s);
} // shape_traits::get_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the highest X-coordinate covered by the bounding box of this
 *        shape_traits.
 * \param s The shape.
 */
template<typename T>
bear::universe::coordinate_type
bear::universe::shape_traits<T>::get_right( const shape_type& s )
{
  return get_left(s) + get_width(s);
} // shape_traits::get_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the highest Y-coordinate covered by the bounding box of this
 *        shape_traits.
 * \param s The shape.
 */
template<typename T>
bear::universe::coordinate_type
bear::universe::shape_traits<T>::get_bottom( const shape_type& s )
{
  return s.get_bottom();
} // shape_traits::get_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the x-coordinate of the center of this shape_traits.
 * \param s The shape.
 */
template<typename T>
bear::universe::coordinate_type
bear::universe::shape_traits<T>::get_horizontal_middle
( const shape_type& s )
{
  return get_left(s) + get_width(s) / 2;
} // shape_traits::get_horizontal_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the Y-coordinate of the center of this shape_traits.
 * \param s The shape.
 */
template<typename T>
bear::universe::coordinate_type
bear::universe::shape_traits<T>::get_vertical_middle
( const shape_type& s )
{
  return get_bottom(s) + get_height(s) / 2;
} // shape_traits::get_vertical_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the top left corner of the shape.
 * \param s The shape.
 */
template<typename T>
bear::universe::position_type
bear::universe::shape_traits<T>::get_top_left( const shape_type& s )
{
  return position_type( get_left(s), get_top(s) );
} // shape_traits::get_top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the middle of the top edge of the shape.
 * \param s The shape.
 */
template<typename T>
bear::universe::position_type
bear::universe::shape_traits<T>::get_top_middle( const shape_type& s )
{
  return position_type( get_center(s).x, get_top(s) );
} // shape_traits::get_top_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the top right corner of the shape.
 * \param s The shape.
 */
template<typename T>
bear::universe::position_type
bear::universe::shape_traits<T>::get_top_right( const shape_type& s )
{
  return position_type( get_right(s), get_top(s) );
} // shape_traits::get_top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the bottom left corner of the shape.
 * \param s The shape.
 */
template<typename T>
bear::universe::position_type
bear::universe::shape_traits<T>::get_bottom_left( const shape_type& s )
{
  return position_type( get_left(s), get_bottom(s) );
} // shape_traits::get_bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the middle of the bottom edge of the shape.
 * \param s The shape.
 */
template<typename T>
bear::universe::position_type
bear::universe::shape_traits<T>::get_bottom_middle( const shape_type& s )
{
  return position_type( get_center(s).x, get_bottom(s) );
} // shape_traits::get_bottom_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the bottom right corner of the shape.
 * \param s The shape.
 */
template<typename T>
bear::universe::position_type
bear::universe::shape_traits<T>::get_bottom_right( const shape_type& s )
{
  return position_type( get_right(s), get_bottom(s) );
} // shape_traits::get_bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the middle of the left edge of the shape.
 * \param s The shape.
 */
template<typename T>
bear::universe::position_type
bear::universe::shape_traits<T>::get_left_middle( const shape_type& s )
{
  return position_type( get_left(s), get_center(s).y );
} // shape_traits::get_left_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position of the middle of the right edge of the shape.
 * \param s The shape.
 */
template<typename T>
bear::universe::position_type
bear::universe::shape_traits<T>::get_right_middle( const shape_type& s )
{
  return position_type( get_right(s), get_center(s).y );
} // shape_traits::get_right_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the position, in the world, of the center of mass.
 * \param s The shape.
 */
template<typename T>
bear::universe::position_type
bear::universe::shape_traits<T>::get_center( const shape_type& s )
{
  return get_bottom_left(s) + get_size(s) / 2;
} // shape_traits::get_center()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its center is at a given position.
 * \param s The shape.
 * \param pos The new center of mass.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_center
( shape_type& s, const position_type& pos )
{
  set_bottom_left( s, pos - get_size(s) / 2 );
} // shape_traits::set_center()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its center is at a given position.
 * \param s The shape.
 * \param x The new x_coordinate of the center of mass.
 * \param y The new y-coordinate of the center of mass.
 */
template<typename T>
void bear::universe::shape_traits<T>::set_center
( shape_type& s, coordinate_type x, coordinate_type y )
{
  set_center( s, position_type(x, y) );
} // shape_traits::set_center()
