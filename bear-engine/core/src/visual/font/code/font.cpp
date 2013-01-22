/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::font class.
 * \author Julien Jorge
 */
#include "visual/font/font.hpp"

#include "visual/font/base_font.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs a font with no implementation. This instance won't have any
 *        visual.
 */
bear::visual::font::font()
  : m_impl(NULL), m_size(0)
{

} // font::font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The implementation of this instance. This implementation will be
 *        shared with the copies of this instance. None of them will ever delete
 *        the implementation.
 * \param s The size of the text displayed with this font.
 */
bear::visual::font::font( base_font_pointer f, size_type size )
  : m_impl( f ), m_size( size )
{

} // font::font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the metrics of a glyph.
 * \param c The glyph for which we want the size.
 */
bear::visual::glyph_metrics
bear::visual::font::get_metrics( charset::char_type c ) const
{
  glyph_metrics result;

  if ( m_impl != NULL )
    result = m_impl->get_metrics( c );

  return result;
} // font::get_metrics()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the spacing between two lines of text displayed with this
 *        font.
 */
bear::visual::size_type bear::visual::font::get_line_spacing() const
{
  return get_size() * 1.2;
} // font::get_line_spacing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the size of the characters in the font.
 */
bear::visual::size_type bear::visual::font::get_size() const
{
  if ( m_impl == NULL )
    return 0;
  else
    return m_impl->get_size();
} // font::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the sprite of a character.
 * \param character The character to get.
 */
bear::visual::sprite
bear::visual::font::get_sprite( charset::char_type character ) const
{
  sprite result;

  if ( m_impl != NULL )
    result = m_impl->get_sprite( character );

  return result;
} // font::get_sprite()
