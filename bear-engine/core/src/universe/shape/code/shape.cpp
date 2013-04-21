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
 * \brief Instantiates a copy of the implementation of this shape. The caller
 *        has the responsability to delete the returned instance.
 */
bear::universe::shape_base* bear::universe::shape::clone_impl() const
{
  if ( m_impl == NULL )
    return NULL;
  else
    return m_impl->clone();
} // shape::clone_impl()
