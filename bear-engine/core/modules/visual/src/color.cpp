/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::color class.
 * \author Julien Jorge
 */
#include "bear/visual/color.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::visual::color::color()
{
  // nothing to do
} // color::color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param c A color from libclaw.
 */
bear::visual::color::color( const claw::graphic::rgba_pixel& c )
  : claw::graphic::rgba_pixel(c)
{

} // color::color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param c A color.
 */
bear::visual::color::color( const color& c )
{
  set_red_intensity( c.get_red_intensity() );
  set_green_intensity( c.get_green_intensity() );
  set_blue_intensity( c.get_blue_intensity() );
  set_opacity( c.get_opacity() );
} // color::color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param c The name of the color.
 */
bear::visual::color::color( const std::string& c )
  : claw::graphic::rgba_pixel(c)
{
} // color::color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param r Intensity of the red component.
 * \param g Intensity of the green component.
 * \param b Intensity of the blue component.
 */
bear::visual::color::color
( component_type r, component_type g, component_type b )
  : claw::graphic::rgba_pixel
    (r, g, b, std::numeric_limits<component_type>::max())
{

} // color::color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param r Intensity of the red component.
 * \param g Intensity of the green component.
 * \param b Intensity of the blue component.
 * \param o opacity.
 */
bear::visual::color::color
( component_type r, component_type g, component_type b, component_type o )
  : claw::graphic::rgba_pixel(r, g, b, o)
{
} // color::color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the color.
 * \param red The red intensity of the component.
 * \param green The green intensity of the component.
 * \param blue The blue intensity of the component.
 * \param opacity The opacity of the component.
 */
void bear::visual::color::set
( double red, double green, double blue, double opacity )
{
  set_red_intensity(red);
  set_green_intensity(green);
  set_blue_intensity(blue);
  set_opacity(opacity);
} // color::set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the intensity of the red component.
 * \param i The intensity of the component.
 */
void bear::visual::color::set_red_intensity( double i )
{
  const component_type max_comp = std::numeric_limits<component_type>::max();

  if ( i < 0 )
    components.red = 0;
  else if ( i > 1 )
    components.red = max_comp;
  else
    components.red = max_comp * i;
} // color::set_red_intensity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the intensity of the green component.
 * \param i The intensity of the component.
 */
void bear::visual::color::set_green_intensity( double i )
{
  const component_type max_comp = std::numeric_limits<component_type>::max();

  if ( i < 0 )
    components.green = 0;
  else if ( i > 1 )
    components.green = max_comp;
  else
    components.green = max_comp * i;
} // color::set_green_intensity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the intensity of the blue component.
 * \param i The intensity of the component.
 */
void bear::visual::color::set_blue_intensity( double i )
{
  const component_type max_comp = std::numeric_limits<component_type>::max();

  if ( i < 0 )
    components.blue = 0;
  else if ( i > 1 )
    components.blue = max_comp;
  else
    components.blue = max_comp * i;
} // color::set_blue_intensity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the intensity of the transparency.
 * \param i The intensity.
 */
void bear::visual::color::set_opacity( double o )
{
  const component_type max_comp = std::numeric_limits<component_type>::max();

  if ( o < 0 )
    components.alpha = 0;
  else if ( o > 1 )
    components.alpha = max_comp;
  else
    components.alpha = max_comp * o;
} // color::set_opacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the intensity of the red component.
 */
double bear::visual::color::get_red_intensity() const
{
  const component_type max_comp = std::numeric_limits<component_type>::max();
  
  return (double)components.red / (double)max_comp;
} // color::get_red_intensity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the intensity of the green component.
 */
double bear::visual::color::get_green_intensity() const
{
  const component_type max_comp = std::numeric_limits<component_type>::max();
  
  return (double)components.green / (double)max_comp;
} // color::get_green_intensity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the intensity of the blue component.
 */
double bear::visual::color::get_blue_intensity() const
{
  const component_type max_comp = std::numeric_limits<component_type>::max();
  
  return (double)components.blue / (double)max_comp;
} // color::get_blue_intensity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the intensity of the transparency.
 */
double bear::visual::color::get_opacity() const
{
  const component_type max_comp = std::numeric_limits<component_type>::max();
  
  return (double)components.alpha / (double)max_comp;
} // color::get_opacity()
