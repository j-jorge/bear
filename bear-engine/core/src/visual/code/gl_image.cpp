/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::image class.
 * \author Julien Jorge
 */
#include "visual/gl_image.hpp"

#include "visual/gl_renderer.hpp"

#include <climits>
#include <limits>

#include <claw/exception.hpp>
#include <claw/assert.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs an image of a given size.
 * \param width The width of the image.
 * \param height The height of the image.
 */
bear::visual::gl_image::gl_image( unsigned int width, unsigned int height )
  : m_texture_id(0), m_size(width, height), m_has_transparency(false)
{
  create_texture();
} // gl_image::gl_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor with a claw::graphic::gl_image object.
 * \param data The image to copy.
 */
bear::visual::gl_image::gl_image(const claw::graphic::image& data)
  : m_texture_id(0), m_size(data.width(), data.height()),
    m_has_transparency(false)
{
  create_texture();
  copy_scanlines(data);
} // gl_image::gl_image() [claw::graphic::gl_image]

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::visual::gl_image::~gl_image()
{
  gl_renderer::get_instance().delete_texture( m_texture_id );
} // gl_image::~gl_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get OpenGL texture identifier.
 */
GLuint bear::visual::gl_image::texture_id() const
{
  return m_texture_id;
} // gl_image::texture_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get image's size.
 */
claw::math::coordinate_2d<unsigned int> bear::visual::gl_image::size() const
{
  return m_size;
} // gl_image::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the image has transparent pixels.
 */
bool bear::visual::gl_image::has_transparency() const
{
  return m_has_transparency;
} // gl_image::has_transparency()

/*----------------------------------------------------------------------------*/
/**
 * \brief Replaces a portion of this image with a given data.
 * \param data The pixels to copy in the image.
 * \param pos The position in the image where data must be copied.
 */
void bear::visual::gl_image::draw
 ( const claw::graphic::image& data,
   claw::math::coordinate_2d<unsigned int> pos )
{
  m_has_transparency =
    gl_renderer::get_instance().draw_texture( m_texture_id, data, pos );
} // gl_image::draw()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reads the pixel colors of the textures.
 * \return An image of the texture.
 */
claw::graphic::image bear::visual::gl_image::read() const
{
  return gl_renderer::get_instance().read_texture( m_texture_id, m_size );
} // gl_image::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an empty OpenGL texture.
 */
void bear::visual::gl_image::create_texture()
{
  m_texture_id = gl_renderer::get_instance().create_texture( m_size );
} // gl_image::create_texture()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the content of an image to the texture.
 * \param data The image to copy.
 */
void bear::visual::gl_image::copy_scanlines( const claw::graphic::image& data )
{
  draw( data, claw::math::coordinate_2d<unsigned int>( 0, 0 ) );
} // gl_image::copy_scanlines()
