/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::bitmap_font class.
 * \author Sebastien Angibaud
 */
#include "bear/visual/font/bitmap_font.hpp"

#include "bear/visual/font/bitmap_charmap.hpp"

#include <claw/assert.hpp>
#include <claw/logger/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param characters The description of the characters in the font. The size of
 *        the font will be set to characters.size.y.
 */
bear::visual::bitmap_font::bitmap_font( const bitmap_charmap& characters )
{
  CLAW_PRECOND( !characters.characters.empty() );

  make_sprites( characters, characters.size.y );
  make_missing( characters, characters.size.y );
} // bitmap_font::bitmap_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param characters The description of the characters in the font.
 * \param size The size of the font.
 */
bear::visual::bitmap_font::bitmap_font
( const bitmap_charmap& characters, size_type size )
{
  CLAW_PRECOND( !characters.characters.empty() );

  make_sprites( characters, size );
  make_missing( characters, size );
} // bitmap_font::bitmap_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the size of the font.
 */
bear::visual::size_type bear::visual::bitmap_font::get_size() const
{
  return m_missing.height();
} // bitmap_font::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprite of a character.
 * \param character The character to get.
 */
bear::visual::sprite
bear::visual::bitmap_font::get_sprite( charset::char_type character )
{
  const std::map<charset::char_type, sprite>::const_iterator it =
    m_characters.find(character);

  sprite result;

  if ( it == m_characters.end() )
    {
      result = m_missing;

      if ( (character == ' ')
           || (character == ' ') /* non breakable space */ )
        result.set_opacity(0);
      else
        {
          claw::logger << claw::log_warning << "Character not found '"
                       << character << "' (" << (int)character
                       << ")." << std::endl;
#ifdef NDEBUG
          result.set_opacity(0);
#endif
        }
    }
  else
    result = it->second;

  return result;
} // bitmap_font::get_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the metrics of the glyph of a character.
 * \param character The character to get.
 */
bear::visual::glyph_metrics
bear::visual::bitmap_font::get_metrics( charset::char_type character )
{
  return glyph_metrics
    ( get_sprite(character).get_size(), size_box_type(0, 0) );
} // bitmap_font::get_metrics()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the sprites for the characters.
 * \param characters The description of the characters in the font.
 * \param size The size of the font.
 */
void bear::visual::bitmap_font::make_sprites
( const bitmap_charmap& characters, double size )
{
  std::map
    <charset::char_type, bitmap_charmap::char_position>::const_iterator it;

  for (it=characters.characters.begin(); it!=characters.characters.end(); ++it)
    if ( it->second.image_index < characters.font_images.size() )
      if ( it->second.position.x + characters.size.x
           < characters.font_images[it->second.image_index].width() )
      if ( it->second.position.y + characters.size.y
           < characters.font_images[it->second.image_index].height() )
        {
          const claw::math::rectangle<unsigned int> clip
            ( it->second.position, characters.size );

          sprite s( characters.font_images[it->second.image_index], clip );
          s.set_size( s.width() * size / s.height(), size );

          m_characters[it->first] = s;
        }
} // bitmap_font::make_sprites()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the sprite for a missing character.
 * \param characters The description of the characters in the font.
 * \param size The size of the font.
 */
void bear::visual::bitmap_font::make_missing
( const bitmap_charmap& characters, double size )
{
  CLAW_PRECOND( !characters.characters.empty() );

  claw::math::rectangle<unsigned int> clip
    ( 0, 0, characters.font_images[0].width(),
      characters.font_images[0].height() );

  m_missing = sprite(characters.font_images[0], clip );

  size_box_type ref_size;

  if ( m_characters.empty() )
    ref_size = size_box_type( size, size );
  else
    ref_size = m_characters.begin()->second.get_size();

  m_missing.set_size( ref_size.x * size / ref_size.y, ref_size.y );
} // bitmap_font::make_missing()
