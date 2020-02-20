/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::sprite class.
 * \author Julien Jorge
 */
#include "bf/sprite.hpp"

#include "bf/compiled_file.hpp"
#include "bf/compilation_context.hpp"
#include "bf/path_configuration.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bf::sprite::sprite()
  : m_top(0), m_left(0), m_clip_width(0), m_clip_height(0)
{

} // sprite::sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the image resource to use.
 * \param name The new value.
 */
void bf::sprite::set_image_name( const std::string& name )
{
  m_image_name = name;
} // sprite::set_image_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the entry in the spritepos file.
 * \param name The new value.
 */
void bf::sprite::set_spritepos_entry( const std::string& name )
{
  m_spritepos_entry = name;
} // sprite::set_spritepos_entry()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the Y-coordinate in the image resource.
 * \param top The new value.
 */
void bf::sprite::set_top( const unsigned int top )
{
  if ( m_top != top )
    {
      m_top = top;
      m_spritepos_entry.clear();
    }
} // sprite::set_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the X-coordinate in the image resource.
 * \param left The new value.
 */
void bf::sprite::set_left( const unsigned int left )
{
  if ( m_left != left )
    {
      m_left = left;
      m_spritepos_entry.clear();
    }
} // sprite::set_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the width in the image resource.
 * \param width The new value.
 */
void bf::sprite::set_clip_width( const unsigned int width )
{
  if ( m_clip_width != width )
    {
      m_clip_width = width;
      m_spritepos_entry.clear();

      if ( get_auto_size() )
        set_width(width);
    }
} // sprite::set_clip_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the height in the image resource.
 * \param height The new value.
 */
void bf::sprite::set_clip_height( const unsigned int height )
{
  if ( m_clip_height != height )
    {
      m_clip_height = height;
      m_spritepos_entry.clear();

      if ( get_auto_size() )
        set_height(height);
    }
} // sprite::set_clip_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the rectangle in the image resource.
 * \param r The rectangle.
 */
void
bf::sprite::set_clip_rectangle( const claw::math::rectangle<unsigned int>& r )
{
  set_left(r.position.x);
  set_top(r.position.y);
  set_clip_width(r.width);
  set_clip_height(r.height);
} // sprite::set_clip_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the left x-ccordinate of the sprite.
 */
unsigned int bf::sprite::get_left() const
{
  return m_left;
} // sprite::get_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the top y-coordinate of the sprite.
 */
unsigned int bf::sprite::get_top() const
{
  return m_top;
} // sprite::get_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of the sprite in the image source.
 */
unsigned int bf::sprite::get_clip_width() const
{
  return m_clip_width;
} // sprite::get_clip_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of the sprite in the image source.
 */
unsigned int bf::sprite::get_clip_height() const
{
  return m_clip_height;
} // sprite::get_clip_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the rectangle in the image resource.
 */
claw::math::rectangle<unsigned int> bf::sprite::get_clip_rectangle() const
{
  return claw::math::rectangle<unsigned int>
    ( get_left(), get_top(), get_clip_width(), get_clip_height() );
} // sprite::get_clip_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the image name of the sprite.
 */
const std::string& bf::sprite::get_image_name() const
{
  return m_image_name;
} // sprite::get_image_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the entry in the spritepos file.
 */
const std::string& bf::sprite::get_spritepos_entry() const
{
  return m_spritepos_entry;
} // sprite::get_spritepos_entry()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the sprite.
 * \param f The stream in which we write the compiled sprite.
 * \param c The context in which the compilation is done.
 */
void bf::sprite::compile( compiled_file& f, compilation_context& c ) const
{
  std::string image_path(m_image_name);

  if ( path_configuration::get_instance().expand_file_name
       ( image_path, c.get_workspace_name() ) )
    path_configuration::get_instance().get_relative_path
      ( image_path, c.get_workspace_name() );

  const compilation_context::rectangle r
    ( c.get_opaque_rectangle( *this, image_path ) );

  f << image_path << m_left << m_top << m_clip_width << m_clip_height
    << r.left() << r.bottom() << r.right() << r.top();

  bitmap_rendering_attributes::compile(f);
} // sprite::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two sprites are the same.
 * \param that The other sprite.
 */
bool bf::sprite::operator==( const sprite& that ) const
{
  return that.bitmap_rendering_attributes::operator==(*this)
    && (m_image_name == that.m_image_name)
    && (m_top == that.m_top)
    && (m_left == that.m_left)
    && (m_clip_width == that.m_clip_width)
    && (m_clip_height == that.m_clip_height);
} // sprite::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two sprites are different.
 * \param that The other sprite.
 */
bool bf::sprite::operator!=( const sprite& that ) const
{
  return !(*this == that);
} // sprite::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare with an other sprite in lexicographic order of their
 *        attributes.
 * \param that The other sprite.
 */
bool bf::sprite::operator<( const sprite& that ) const
{
  if ( m_image_name != that.m_image_name )
    return m_image_name < that.m_image_name;

  if ( m_top != that.m_top )
    return m_top < that.m_top;

  if ( m_left != that.m_left )
    return m_left < that.m_left;

  if (m_clip_width != that.m_clip_width)
    return m_clip_width < that.m_clip_width;

  if ( m_clip_height != that.m_clip_height )
    return m_clip_height < that.m_clip_height;

  return that.bitmap_rendering_attributes::operator<(*this);
} // sprite::operator<()

