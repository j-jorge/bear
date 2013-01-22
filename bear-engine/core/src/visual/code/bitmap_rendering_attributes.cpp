/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the  bitmap_rendering_attributes class.
 * \author Julien Jorge
 */
#include "visual/bitmap_rendering_attributes.hpp"

#include <claw/assert.hpp>
#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::visual::bitmap_rendering_attributes::bitmap_rendering_attributes()
  : m_size(0, 0), m_flip(false), m_mirror(false), m_opacity(1),
    m_red_intensity(1), m_green_intensity(1), m_blue_intensity(1), m_angle(0)
{

} // bitmap_rendering_attributes::bitmap_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param s The size.
 */
bear::visual::bitmap_rendering_attributes::bitmap_rendering_attributes
( const size_box_type& s )
  : m_size(s), m_flip(false), m_mirror(false), m_opacity(1),
    m_red_intensity(1), m_green_intensity(1), m_blue_intensity(1), m_angle(0)
{

} // bitmap_rendering_attributes::bitmap_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the opacity translucy.
 */
double bear::visual::bitmap_rendering_attributes::get_opacity() const
{
  return m_opacity;
} // bitmap_rendering_attributes::get_opacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the opacity.
 */
void bear::visual::bitmap_rendering_attributes::set_opacity( double opacity )
{
  if ( opacity > 1 )
    m_opacity = 1;
  else if ( opacity < 0 )
    m_opacity = 0;
  else
    m_opacity = opacity;
} // bitmap_rendering_attributes::set_opacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the intensity of the red channel.
 */
double bear::visual::bitmap_rendering_attributes::get_red_intensity() const
{
  return m_red_intensity;
} // bitmap_rendering_attributes::get_red_intensity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the intensity of the green channel.
 */
double bear::visual::bitmap_rendering_attributes::get_green_intensity() const
{
  return m_green_intensity;
} // bitmap_rendering_attributes::get_green_intensity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the intensity of the blue channel.
 */
double bear::visual::bitmap_rendering_attributes::get_blue_intensity() const
{
  return m_blue_intensity;
} // bitmap_rendering_attributes::get_blue_intensity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the intensity of the channels.
 * \param r Value on the r component.
 * \param g Value on the r component.
 * \param b Value on the r component.
 */
void bear::visual::bitmap_rendering_attributes::set_intensity
( double r, double g, double b )
{
  if (r > 1)
    m_red_intensity = 1;
  else if (r < 0)
    m_red_intensity = 0;
  else
    m_red_intensity = r;

  if (g > 1)
    m_green_intensity = 1;
  else if (g < 0)
    m_green_intensity = 0;
  else
    m_green_intensity = g;

  if (b > 1)
    m_blue_intensity = 1;
  else if (b < 0)
    m_blue_intensity = 0;
  else
    m_blue_intensity = b;
} // bitmap_rendering_attributes::set_intensity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the intensity of the red channel.
 * \param i The intensity.
 */
void bear::visual::bitmap_rendering_attributes::set_red_intensity( double i )
{
  if (i > 1)
    m_red_intensity = 1;
  else if (i < 0)
    m_red_intensity = 0;
  else
    m_red_intensity = i;
} // bitmap_rendering_attributes::set_red_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the intensity of the green channel.
 * \param i The intensity.
 */
void bear::visual::bitmap_rendering_attributes::set_green_intensity( double i )
{
  if (i > 1)
    m_green_intensity = 1;
  else if (i < 0)
    m_green_intensity = 0;
  else
    m_green_intensity = i;
} // bitmap_rendering_attributes::set_green_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the intensity of the blue channel.
 * \param i The intensity.
 */
void bear::visual::bitmap_rendering_attributes::set_blue_intensity( double i )
{
  if (i > 1)
    m_blue_intensity = 1;
  else if (i < 0)
    m_blue_intensity = 0;
  else
    m_blue_intensity = i;
} // bitmap_rendering_attributes::set_blue_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Merges the intensity of the color channels and the opacity of the
 *        element with a given color.
 * \param c The color.
 */
void bear::visual::bitmap_rendering_attributes::colorize( color_type c )
{
  const color_type::component_type component_max
    ( std::numeric_limits<color_type::component_type>::max() );

  set_intensity
    ( get_red_intensity() * (double)c.components.red / component_max,
      get_green_intensity() * (double)c.components.green / component_max,
      get_blue_intensity() * (double)c.components.blue / component_max );

  set_opacity( get_opacity() * (double)c.components.alpha / component_max );
} // bitmap_rendering_attributes::colorize()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the color.
 * \param c The color.
 */
void bear::visual::bitmap_rendering_attributes::set_color(const color_type& c)
{
  set_intensity
    ( c.get_red_intensity(), c.get_green_intensity(), c.get_blue_intensity());
  set_opacity( c.get_opacity() );
} // bitmap_rendering_attributes::set_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the intensity of the color channels and the opacity of this
 *        element, represented by a color instance.
 */
bear::visual::color_type
bear::visual::bitmap_rendering_attributes::get_color() const
{
  color_type result;

  result.set_red_intensity( get_red_intensity() );
  result.set_green_intensity( get_green_intensity() );
  result.set_blue_intensity( get_blue_intensity() );
  result.set_opacity( get_opacity() );

  return result;
} // bitmap_rendering_attributes::get_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size.
 */
const bear::visual::size_box_type&
bear::visual::bitmap_rendering_attributes::get_size() const
{
  return m_size;
} // bitmap_rendering_attributes::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size.
 * \param s The new size.
 */
void
bear::visual::bitmap_rendering_attributes::set_size( const size_box_type& s )
{
  m_size = s;
} // bitmap_rendering_attributes::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size.
 * \param w The new width.
 * \param h The new height.
 */
void bear::visual::bitmap_rendering_attributes::set_size
( size_type w, size_type h )
{
  m_size.set(w, h);
} // bitmap_rendering_attributes::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the width.
 * \param w The new width.
 */
void bear::visual::bitmap_rendering_attributes::set_width( size_type w )
{
  m_size.x = w;
} // bitmap_rendering_attributes::set_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the height.
 * \param h The new height.
 */
void bear::visual::bitmap_rendering_attributes::set_height( size_type h )
{
  m_size.y = h;
} // bitmap_rendering_attributes::set_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width.
 */
bear::visual::size_type bear::visual::bitmap_rendering_attributes::width() const
{
  return m_size.x;
} // bitmap_rendering_attributes::width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height.
 */
bear::visual::size_type
bear::visual::bitmap_rendering_attributes::height() const
{
  return m_size.y;
} // bitmap_rendering_attributes::height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the angle.
 * \param a a Rotate the sprite with this angle (in radians and in trigonometric
 *          direction). The center of the rotation is the center of the sprite.
 */
void bear::visual::bitmap_rendering_attributes::set_angle( double a )
{
  m_angle = a;
} // bitmap_rendering_attributes::set_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the angle.
 */
double bear::visual::bitmap_rendering_attributes::get_angle() const
{
  return m_angle;
} // bitmap_rendering_attributes::get_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set vertical inversion.
 * \param b The new value for the inversion.
 */
void bear::visual::bitmap_rendering_attributes::flip( bool b )
{
  m_flip = b;
} // bitmap_rendering_attributes::flip()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set horizontal inversion.
 * \param b The new value for the inversion.
 */
void bear::visual::bitmap_rendering_attributes::mirror( bool b )
{
  m_mirror = b;
} // bitmap_rendering_attributes::mirror()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get vertical inversion.
 */
bool bear::visual::bitmap_rendering_attributes::is_flipped() const
{
  return m_flip;
} // bitmap_rendering_attributes::is_flipped()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get horizontal inversion.
 */
bool bear::visual::bitmap_rendering_attributes::is_mirrored() const
{
  return m_mirror;
} // bitmap_rendering_attributes::is_mirrored()

/*----------------------------------------------------------------------------*/
/**
 * \brief Combine with an other set of attributes.
 * \param that The attributes to combine with.
 *
 * The attributes changed by this method are : is_flipped(), is_mirrored(), the
 * intensities, the opacity and the angle. The size is not changed.
 */
void bear::visual::bitmap_rendering_attributes::combine
( const bitmap_rendering_attributes& that )
{
  flip( that.is_flipped() ^ is_flipped() );
  mirror( that.is_mirrored() ^ is_mirrored() );
  set_intensity
    ( that.get_red_intensity() * get_red_intensity(),
      that.get_green_intensity() * get_green_intensity(),
      that.get_blue_intensity() * get_blue_intensity()
      );
  set_opacity( that.get_opacity() * get_opacity() );
  set_angle( that.get_angle() + get_angle() );
} // bitmap_rendering_attributes::combine()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the intensity and the opacities to a given color.
 * \param c The color.
 */
bear::visual::color_type
bear::visual::bitmap_rendering_attributes::convert_color
( const color_type& c ) const
{
  return
    color_type
    ( c.components.red * get_red_intensity(),
      c.components.green * get_green_intensity(),
      c.components.blue * get_blue_intensity(),
      c.components.alpha * get_opacity() );
} // bitmap_rendering_attributes::convert_color()
