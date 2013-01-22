/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the  bitmap_rendering_attributes class.
 * \author Julien Jorge
 */
#include "bf/bitmap_rendering_attributes.hpp"

#include "bf/compiled_file.hpp"

#include <claw/assert.hpp>
#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::bitmap_rendering_attributes::bitmap_rendering_attributes()
  : m_size(0, 0), m_flip(trinary_logic::boolean_false),
    m_mirror(trinary_logic::boolean_false), 
    m_color(1,1,1,1), m_angle(0),
    m_auto_size(true)
{

} // bitmap_rendering_attributes::bitmap_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two bitmap_rendering_attributes are the same.
 * \param that The instance to compare to.
 */
bool bf::bitmap_rendering_attributes::operator==
( const bitmap_rendering_attributes& that ) const
{
  return (m_size == that.m_size)
    && (m_flip == that.m_flip)
    && (m_mirror == that.m_mirror)
    && (m_color == that.m_color)
    && (m_angle == that.m_angle)
    && (m_auto_size == that.m_auto_size);
} // bitmap_rendering_attributes::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two bitmap_rendering_attributes are different.
 * \param that The instance to compare to.
 */
bool bf::bitmap_rendering_attributes::operator!=
( const bitmap_rendering_attributes& that ) const
{
  return !(*this == that);
} // bitmap_rendering_attributes::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two bitmap_rendering_attributes in lexicographic order.
 * \param that The instance to compare to.
 */
bool bf::bitmap_rendering_attributes::operator<
( const bitmap_rendering_attributes& that ) const
{
  if (m_size.x != that.m_size.x)
    return m_size.x < that.m_size.x;

  if (m_size.y != that.m_size.y)
    return m_size.y < that.m_size.y;

  if (m_flip != that.m_flip)
    return m_flip < that.m_flip;

  if (m_mirror != that.m_mirror)
    return m_mirror < that.m_mirror;

  if (m_color != that.m_color)
    return m_color < that.m_color;

  if ( m_angle != that.m_angle )
    return m_angle < that.m_angle;

  return m_auto_size < that.m_auto_size;
} // bitmap_rendering_attributes::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the color.
 */
const bf::color& bf::bitmap_rendering_attributes::get_color() const
{
  return m_color;
} // bitmap_rendering_attributes::get_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the color.
 */
bf::color& bf::bitmap_rendering_attributes::get_color()
{
  return m_color;
} // bitmap_rendering_attributes::get_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the color.
 * \param c The color.
 */
void bf::bitmap_rendering_attributes::set_color( const color& c )
{
  m_color = c;
} // bitmap_rendering_attributes::set_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the size is automatically computed.
 * \param a Tell if the size is automatically computed.
 */
void bf::bitmap_rendering_attributes::set_auto_size( bool a )
{
  m_auto_size = a;
} // bitmap_rendering_attributes::set_auto_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the size is automatically computed.
 */
bool bf::bitmap_rendering_attributes::get_auto_size() const
{
  return m_auto_size;
} // bitmap_rendering_attributes::get_auto_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size.
 */
const claw::math::coordinate_2d<unsigned int>&
bf::bitmap_rendering_attributes::get_size() const
{
  return m_size;
} // bitmap_rendering_attributes::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size.
 * \param s The new size.
 */
void bf::bitmap_rendering_attributes::set_size
( const claw::math::coordinate_2d<unsigned int>& s )
{
  m_size = s;
} // bitmap_rendering_attributes::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size.
 * \param w The new width.
 * \param h The new height.
 */
void bf::bitmap_rendering_attributes::set_size
( unsigned int w, unsigned int h )
{
  m_size.set(w, h);
} // bitmap_rendering_attributes::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the width.
 * \param w The new width.
 */
void bf::bitmap_rendering_attributes::set_width( unsigned int w )
{
  m_size.x = w;
} // bitmap_rendering_attributes::set_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the height.
 * \param h The new height.
 */
void bf::bitmap_rendering_attributes::set_height( unsigned int h )
{
  m_size.y = h;
} // bitmap_rendering_attributes::set_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width.
 */
unsigned int bf::bitmap_rendering_attributes::width() const
{
  return m_size.x;
} // bitmap_rendering_attributes::width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height.
 */
unsigned int bf::bitmap_rendering_attributes::height() const
{
  return m_size.y;
} // bitmap_rendering_attributes::height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the angle.
 * \param a The new angle.
 */
void bf::bitmap_rendering_attributes::set_angle( double a )
{
  m_angle = a;
} // bitmap_rendering_attributes::set_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the angle.
 */
double bf::bitmap_rendering_attributes::get_angle() const
{
  return m_angle;
} // bitmap_rendering_attributes::get_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set vertical inversion.
 * \param b The new value for the inversion.
 */
void bf::bitmap_rendering_attributes::flip( trinary_logic::value_type b )
{
  m_flip = b;
} // bitmap_rendering_attributes::flip()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set horizontal inversion.
 * \param b The new value for the inversion.
 */
void bf::bitmap_rendering_attributes::mirror( trinary_logic::value_type b )
{
  m_mirror = b;
} // bitmap_rendering_attributes::mirror()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get vertical inversion.
 */
bf::trinary_logic::value_type
bf::bitmap_rendering_attributes::get_flipped_status() const
{
  return m_flip;
} // bitmap_rendering_attributes::get_flipped_status()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get horizontal inversion.
 */
bf::trinary_logic::value_type
bf::bitmap_rendering_attributes::get_mirrored_status() const
{
  return m_mirror;
} // bitmap_rendering_attributes::get_mirrored_status()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the attributes.
 * \param f The stream in which we write the compilation.
 */
void bf::bitmap_rendering_attributes::compile( compiled_file& f ) const
{
  f << m_size.x << m_size.y << trinary_logic::to_random_bool(m_mirror)
    << trinary_logic::to_random_bool(m_flip) << m_color.get_opacity()
    << m_color.get_red_intensity() << m_color.get_green_intensity() 
    << m_color.get_blue_intensity() << m_angle;
} // bitmap_rendering_attributes::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the values of an other bitmap_rendering_attributes in this one.
 * \param that The bitmap_rendering_attributes from which we copy.
 */
bf::bitmap_rendering_attributes& bf::bitmap_rendering_attributes::assign
( const bitmap_rendering_attributes& that )
{
  return *this = that;
} // bitmap_rendering_attributes::assign()

/*----------------------------------------------------------------------------*/
/**
 * \brief Combine with an other set of attributes.
 * \param that The attributes to combine with.
 * The attributes changed by this method are : get_flipped_status(),
 * get_mirrored_status(), the intensities, the opacity and the angle.
 * The size is not changed.
 */
void bf::bitmap_rendering_attributes::combine
( const bitmap_rendering_attributes& that )
{
  flip( trinary_logic::from_bool
        (trinary_logic::to_bool(that.get_flipped_status()) ^
         trinary_logic::to_bool(get_flipped_status())) );
  mirror( trinary_logic::from_bool
          (trinary_logic::to_bool(that.get_mirrored_status()) ^
           trinary_logic::to_bool(get_mirrored_status())) );
  m_color.set_intensity
    ( that.get_color().get_red_intensity() * m_color.get_red_intensity(),
      that.get_color().get_green_intensity() * m_color.get_green_intensity(),
      that.get_color().get_blue_intensity() * m_color.get_blue_intensity()
      );
  m_color.set_opacity( that.get_color().get_opacity() * m_color.get_opacity() );
  set_angle( that.get_angle() + get_angle() );
} // bitmap_rendering_attributes::combine()
