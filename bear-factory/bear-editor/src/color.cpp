/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::color class.
 * \author Sébastien Angibaud
 */
#include "bf/color.hpp"

#include "bf/compiled_file.hpp"
#include "bf/path_configuration.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bf::color::color()
  : m_opacity(1),
    m_red_intensity(0), m_green_intensity(0), m_blue_intensity(0)
{

} // color::color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param red The red intensity.
 * \param green The green intensity.
 * \param blue The blue intensity.
 * \param opacity The opacity. 
 */
bf::color::color(double red, double green, double blue, double opacity)
  : m_opacity(opacity),
    m_red_intensity(red), m_green_intensity(green), m_blue_intensity(blue)
{

} // color::color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the opacity translucy.
 */
double bf::color::get_opacity() const
{
  return m_opacity;
} // color::get_opacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the opacity translucy.
 */
void
bf::color::set_opacity( double opacity )
{
  m_opacity = opacity;
} // color::set_opacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the intensity of the red channel.
 */
double bf::color::get_red_intensity() const
{
  return m_red_intensity;
} // color::get_red_intensity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the intensity of the green channel.
 */
double bf::color::get_green_intensity() const
{
  return m_green_intensity;
} // color::get_green_intensity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the intensity of the blue channel.
 */
double bf::color::get_blue_intensity() const
{
  return m_blue_intensity;
} // color::get_blue_intensity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the intensity of the channels.
 * \param r Value on the r component.
 * \param g Value on the r component.
 * \param b Value on the r component.
 */
void bf::color::set_intensity
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
} // color::set_intensity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the color.
 * \param f The stream in which we write the compiled color.
 */
void bf::color::compile( compiled_file& f ) const
{
  f << m_opacity << m_red_intensity << m_green_intensity << m_blue_intensity;
} // color::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two colors are the same.
 * \param that The instance to compare to.
 */
bool bf::color::operator==( const color& that ) const
{
  return (m_opacity == that.m_opacity)
    && (m_red_intensity == that.m_red_intensity)
    && (m_green_intensity == that.m_green_intensity)
    && (m_blue_intensity == that.m_blue_intensity);
} // color::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two colors are different.
 * \param that The instance to compare to.
 */
bool bf::color::operator!=( const color& that ) const
{
  return !(*this == that);
} // color::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two colors in lexicographic order.
 * \param that The instance to compare to.
 */
bool bf::color::operator<( const color& that ) const
{
  if (m_opacity != that.m_opacity)
    return m_opacity < that.m_opacity;

  if (m_red_intensity != that.m_red_intensity)
    return m_red_intensity < that.m_red_intensity;

  if (m_green_intensity != that.m_green_intensity)
    return m_green_intensity < that.m_blue_intensity;

  return m_blue_intensity < that.m_blue_intensity;
} // color::operator<()
