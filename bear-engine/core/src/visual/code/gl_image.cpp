/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::image class.
 * \author Julien Jorge
 */
#include <climits>
#include <limits>

#include "visual/gl_image.hpp"
#include "visual/gl_error.hpp"

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
  glDeleteTextures(1, &m_texture_id);
} // gl_image::~gl_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get OpenGL texture identifier.
 */
GLuint bear::visual::gl_image::texture_id() const
{
  if ( !glIsTexture( m_texture_id ) )
    claw::logger << claw::log_error << "Requesting invalid texture: "
                 << m_texture_id << std::endl;

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
  glBindTexture(GL_TEXTURE_2D, m_texture_id);

  const claw::graphic::rgba_pixel_8::component_type opaque =
    std::numeric_limits<claw::graphic::rgba_pixel_8::component_type>::max();

  const std::size_t pixels_count( data.width() * data.height() );
  claw::graphic::rgba_pixel_8* const pixels =
    new claw::graphic::rgba_pixel_8[ pixels_count ];

  std::copy( data.begin(), data.end(), pixels );

  glTexSubImage2D
    ( GL_TEXTURE_2D, 0, pos.x, pos.y, data.width(), data.height(), GL_RGBA,
      GL_UNSIGNED_BYTE, pixels );
  VISUAL_GL_ERROR_THROW();

  for ( claw::graphic::rgba_pixel_8* p = pixels;
        (p != pixels + pixels_count) && !m_has_transparency; ++p )
    m_has_transparency = p->components.alpha != opaque;

  delete[] pixels;
} // gl_image::draw()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an empty OpenGL texture.
 */
void bear::visual::gl_image::create_texture()
{
  unsigned int v;
  for ( v=1; (v < m_size.x) && /* overflow */ (v != 0); v *= 2 ) { }

  m_size.x = v;

  for ( v=1; (v < m_size.y) && /* overflow */ (v != 0); v *= 2 ) { }

  m_size.y = v;

  glGenTextures(1, &m_texture_id);
  glBindTexture(GL_TEXTURE_2D, m_texture_id);

  VISUAL_GL_ERROR_THROW();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL );
  VISUAL_GL_ERROR_THROW();
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
