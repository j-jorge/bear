/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::image class.
 * \author Julien Jorge
 */
#include "visual/image.hpp"

#include "visual/screen.hpp"
#include "visual/gl_image.hpp"

#include <claw/exception.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::visual::image::image()
  : m_impl(NULL)
{

} // image::image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs an image of a given size.
 * \param width The width of the image.
 * \param height The height of the image.
 */
bear::visual::image::image( unsigned int width, unsigned int height )
  : m_impl(new base_image_ptr(NULL))
{
  switch ( screen::get_sub_system() )
    {
    case screen::screen_gl:
      *m_impl = new gl_image( width, height );
      break;
    case screen::screen_undef:
      claw::exception("screen sub system has not been set.");
    }
} // image::image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor with a claw::graphic::image object.
 * \param data The image to copy.
 */
bear::visual::image::image(const claw::graphic::image& data)
  : m_impl(new base_image_ptr(NULL))
{
  restore(data);
} // image::image() [claw::graphic::image]

/*----------------------------------------------------------------------------*/
/**
 * \brief Delete the data ofthe image.
 */
void bear::visual::image::clear()
{
  if ( m_impl != NULL )
    *m_impl = NULL;
} // image::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Restore the image.
 * \param data The image to restore from.
 */
void bear::visual::image::restore( const claw::graphic::image& data )
{
  if ( m_impl == NULL )
    m_impl = new base_image_ptr(NULL);
  else if (*m_impl != NULL)
    {
      assert( data.width() == width() );
      assert( data.height() == height() );
    }

  switch ( screen::get_sub_system() )
    {
    case screen::screen_gl:
      *m_impl = new gl_image(data);
      break;
    case screen::screen_undef:
      claw::exception("screen sub system has not been set.");
    }
} // image::restore()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get image's width.
 */
unsigned int bear::visual::image::width() const
{
  CLAW_PRECOND( is_valid() );
  return (*m_impl)->width();
} // image::width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get image's height.
 */
unsigned int bear::visual::image::height() const
{
  CLAW_PRECOND( is_valid() );
  return (*m_impl)->height();
} // image::height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get image's size.
 */
claw::math::coordinate_2d<unsigned int> bear::visual::image::size() const
{
  CLAW_PRECOND( is_valid() );
  return (*m_impl)->size();
} // image::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the image has transparent pixels.
 */
bool bear::visual::image::has_transparency() const
{
  if( is_valid() )
    return (*m_impl)->has_transparency();
  else
    return false;
} // image::has_transparency()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the image is initialized.
 */
bool bear::visual::image::is_valid() const
{
  bool result = false;

  if ( m_impl != NULL )
    result = ( *m_impl != NULL );

  return result;
} // image::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Replaces a portion of this image with a given data.
 * \param data The pixels to copy in the image.
 * \param pos The position in the image where data must be copied.
 */
void bear::visual::image::draw
 ( const claw::graphic::image& data,
   claw::math::coordinate_2d<unsigned int> pos )
{
  if ( is_valid() )
    (*m_impl)->draw( data, pos );
} // image::draw()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assigment operator.
 */
const bear::visual::base_image* bear::visual::image::get_impl() const
{
  CLAW_PRECOND( is_valid() );

  return &(**m_impl);
} // image::get_impl()
