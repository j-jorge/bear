/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::shape class.
 * \author Julien Jorge
 */
#include "universe/shape/shape.hpp"

#include "universe/shape/shape_base.hpp"

namespace bear
{
  namespace universe
  {
    class dummy_shape:
      public shape_base
    {
      dummy_shape* clone() const { return const_cast<dummy_shape*>( this ); }

      bool intersects( const rectangle& that ) const { return false; }
      bool intersects( const curved_box& that ) const { return false; }

      coordinate_type do_get_bottom() const { return 0; }
      void do_set_bottom( coordinate_type p ) {}

      coordinate_type do_get_left() const { return 0; }
      void do_set_left( coordinate_type p ) {}

      size_type do_get_width() const { return 0; }
      void do_set_width( size_type s ) {}

      size_type do_get_height() const { return 0; }
      void do_set_height( size_type s ) {}

      bool do_intersects( const shape_base& that ) const { return false; }

    };

    static dummy_shape g_dummy_shape;
  }
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::shape::shape()
  : m_impl( &g_dummy_shape )
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
  : m_impl( that.clone_impl() )
{

} // shape::shape()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::universe::shape::~shape()
{
  if ( m_impl != &g_dummy_shape )
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
  return m_impl->intersects( *that.m_impl );
} // shape::intersects()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the width of the shape.
 */
bear::universe::size_type bear::universe::shape::get_width() const
{
  return m_impl->get_width();
} // shape::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the width of the shape.
 * \param width The width of the shape.
 */
void bear::universe::shape::set_width( size_type width )
{
  m_impl->set_width( width );
} // shape::set_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the height of the shape.
 */
bear::universe::size_type
bear::universe::shape::get_height() const
{
  return m_impl->get_height();
} // shape::get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the height of the shape.
 * \param height The height of the shape.
 */
void bear::universe::shape::set_height( size_type height )
{
  m_impl->set_height( height );
} // shape::set_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the highest Y-coordinate covered by the bounding box of this
 *        shape.
 */
bear::universe::coordinate_type bear::universe::shape::get_bottom() const
{
  return m_impl->get_bottom();
} // shape::get_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its bottom is at a given coordinate.
 * \param pos The new position.
 */
void bear::universe::shape::set_bottom( coordinate_type pos )
{
  m_impl->set_bottom( pos );
} // shape::set_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the lowest X-coordinate covered by the bounding box of this
 *        shape.
 */
bear::universe::coordinate_type bear::universe::shape::get_left() const
{
  return m_impl->get_left();
} // shape::get_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Moves the shape such that its left is at a given coordinate.
 * \param pos The new position.
 */
void bear::universe::shape::set_left( coordinate_type pos )
{
  m_impl->set_left( pos );
} // shape::set_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instantiates a copy of the implementation of this shape. The caller
 *        has the responsability to delete the returned instance.
 */
bear::universe::shape_base* bear::universe::shape::clone_impl() const
{
  return m_impl->clone();
} // shape::clone_impl()
