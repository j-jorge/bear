/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::shape_base class.
 * \author Julien Jorge
 */
#include "universe/shape/shape_base.hpp"

#include "universe/shape/shape_traits.hpp"

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
 * \brief Tells if the bounding box of this shape has an intersection with the
 *        bounding box of another given shape.
 * \param that The other shape.
 */
bool bear::universe::shape_base::bounding_box_intersects
( const shape_base& that ) const
{
  const rectangle_type this_bounding_box
    ( shape_traits<shape_base>::get_bounding_box( *this ) );
  const rectangle_type that_bounding_box
    ( shape_traits<shape_base>::get_bounding_box( that ) );

  if ( this_bounding_box.intersects( that_bounding_box ) )
    {
      const rectangle_type inter =
        this_bounding_box.intersection( that_bounding_box );

      return (inter.width() != 0) && (inter.height() != 0);
    }
  else
    return false;
} // shape_base::bounding_box_intersects()

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
 * \brief Gets the highest Y-coordinate covered by the bounding box of this
 *        shape.
 */
bear::universe::coordinate_type bear::universe::shape_base::get_bottom() const
{
  return do_get_bottom();
} // shape_base::get_bottom()

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
 * \brief Gets the lowest X-coordinate covered by the bounding box of this
 *        shape.
 */
bear::universe::coordinate_type bear::universe::shape_base::get_left() const
{
  return do_get_left();
} // shape_base::get_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its left is at a given coordinate.
 * \param pos The new position.
 */
void bear::universe::shape_base::set_left( coordinate_type pos )
{
  do_set_left( pos );
} // shape_base::set_left()
