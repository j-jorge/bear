/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the  sprite class.
 * \author Julien Jorge
 */
#include "visual/sprite.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::visual::sprite::sprite()
  : m_clip_rectangle(0, 0, 0, 0), m_opaque_rectangle( 0, 0, 0, 0 )
{

} // sprite::sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image for the sprite.
 * \param clip_rectangle Area of "image" for the sprite.
 * \pre clip_rectangle is valid (in the image).
 */
bear::visual::sprite::sprite
( const image& img,
  const claw::math::rectangle<coordinate_type>& clip_rectangle )
  : bitmap_rendering_attributes( clip_rectangle.size() ), m_image(img),
    m_clip_rectangle(clip_rectangle), m_opaque_rectangle(0, 0, 0, 0)
{
  CLAW_PRECOND( m_clip_rectangle.position.x
                + m_clip_rectangle.width <= m_image.width() );
  CLAW_PRECOND( m_clip_rectangle.position.y
                + m_clip_rectangle.height <= m_image.height() );
} // sprite::sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image for the sprite.
 * \param clip_rectangle Area of "image" for the sprite.
 * \param opaque_rectangle An area in the sprite where all pixels are opaques.
 * \pre clip_rectangle is valid (in the image).
 */
bear::visual::sprite::sprite
( const image& img,
  const claw::math::rectangle<coordinate_type>& clip_rectangle,
  const rectangle_type& opaque_rectangle )
  : bitmap_rendering_attributes( clip_rectangle.size() ), m_image(img),
    m_clip_rectangle(clip_rectangle), m_opaque_rectangle( opaque_rectangle )
{
  CLAW_PRECOND( m_clip_rectangle.position.x
                + m_clip_rectangle.width <= m_image.width() );
  CLAW_PRECOND( m_clip_rectangle.position.y
                + m_clip_rectangle.height <= m_image.height() );
  CLAW_PRECOND( m_opaque_rectangle.width() <= m_clip_rectangle.width );
  CLAW_PRECOND( m_opaque_rectangle.height() <= m_clip_rectangle.height );
  CLAW_PRECOND( m_opaque_rectangle.width() >= 0 );
  CLAW_PRECOND( m_opaque_rectangle.height() >= 0 );
} // sprite::sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image for the sprite.
 */
bear::visual::sprite::sprite( const image& img )
  : bitmap_rendering_attributes(img.size()), m_image(img),
    m_clip_rectangle(0, 0, img.width(), img.height()),
    m_opaque_rectangle( 0, 0, 0, 0 )
{

} // sprite::sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the sprite has a transparency.
 */
bool bear::visual::sprite::has_transparency() const
{
  return (get_opacity() != 1) || m_image.has_transparency();
} // sprite::has_transparency()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the rectangular region of the picture used by this sprite.
 */
const claw::math::rectangle<bear::visual::coordinate_type>&
bear::visual::sprite::clip_rectangle() const
{
  return m_clip_rectangle;
} // sprite::clip_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the rectangular region of the picture used by this sprite.
 */
void bear::visual::sprite::set_clip_rectangle
( const claw::math::rectangle<coordinate_type>& clip )
{
  CLAW_PRECOND( clip.position.x + clip.width <= m_image.width() );
  CLAW_PRECOND( clip.position.y + clip.height <= m_image.height() );

  m_clip_rectangle = clip;
} // sprite::set_clip_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the part of the sprite that is fully opaque.
 */
const bear::visual::rectangle_type&
bear::visual::sprite::get_opaque_rectangle() const
{
  return m_opaque_rectangle;
} // sprite::get_opaque_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the part of the sprite that is fully opaque.
 * \param r A fully opaque part of the sprite.
 */
void bear::visual::sprite::set_opaque_rectangle( const rectangle_type& r )
{
  CLAW_PRECOND( r.width() >= 0 );
  CLAW_PRECOND( r.height() >= 0 );

  m_opaque_rectangle = r;
} // sprite::set_opaque_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the picture used by this sprite.
 */
const bear::visual::image& bear::visual::sprite::get_image() const
{
  return m_image;
} // sprite::get_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the sprite is made with a valid image.
 */
bool bear::visual::sprite::is_valid() const
{
  return m_image.is_valid() && (width() >= 0) && (height() >= 0);
} // sprite::is_valid()
