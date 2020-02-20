/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::concept::region class.
 * \author Julien Jorge.
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the region intersects a shape.
 * \param The shape to check.
 */
template<class Shape, class Container>
bool bear::concept::region<Shape, Container>::intersects
( const shape_type& shape ) const
{
  bool result = false;

  typename super::const_iterator it;

  for (it=this->begin(); (it!=this->end()) && !result; ++it)
    result = it->intersects( shape );

  return result;
} // region::intersects()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the region includes a shape.
 * \param The shape to check.
 */
template<class Shape, class Container>
bool bear::concept::region<Shape, Container>::includes
( const shape_type& shape ) const
{
  bool result = false;

  typename super::const_iterator it;

  for (it=this->begin(); (it!=this->end()) && !result; ++it)
    result = it->includes( shape );

  return result;
} // region::includes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the region includes a point.
 * \param The point to check.
 */
template<class Shape, class Container>
template<typename Point>
bool bear::concept::region<Shape, Container>::includes( const Point& p ) const
{
  bool result = false;

  typename super::const_iterator it;

  for (it=this->begin(); (it!=this->end()) && !result; ++it)
    result = it->includes( p );

  return result;
} // region::includes()
