/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::true_type_font class.
 * \author Sebastien Angibaud
 */
#include "visual/font/true_type_font.hpp"

#include <claw/assert.hpp>
#include <claw/exception.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::visual::true_type_font::glyph_sheet::glyph_sheet()
  : m_image( 512, 512 ), m_next_position( 1, 1 ), m_current_line_height(0)
{

} // true_type_font::glyph_sheet::glyph_sheet()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if a given glyph can be added to the sheet.
 * \param c The character to draw.
 * \param face The font face from which we take the glyph.
 */
bool
bear::visual::true_type_font::glyph_sheet::can_draw
( charset::char_type c, const freetype_face& face ) const
{
  const size_box_type glyph_size( face.get_glyph_size(c) );

  if ( m_next_position.x + glyph_size.x < m_image.width() )
    return m_next_position.y + glyph_size.y < m_image.height();
  else
    return m_next_position.y + m_current_line_height + 1 < m_image.height();
} // true_type_font::glyph_sheet::can_draw()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draws a new glyph on this sheet.
 * \param c The character to draw.
 * \param face The font face from which we take the glyph.
 */
void bear::visual::true_type_font::glyph_sheet::draw_character
( charset::char_type c, const freetype_face& face )
{
  CLAW_PRECOND( can_draw( c, face ) );

  const size_box_type glyph_size( face.get_glyph_size(c) );

  if ( m_next_position.x + glyph_size.x >= m_image.width() )
    {
      m_next_position.x = 1;
      m_next_position.y += m_current_line_height + 1;
      m_current_line_height = 0;
    }

  m_image.draw( face.get_glyph( c ), m_next_position );

  character_placement placement;
  placement.clip = clip_rectangle( m_next_position, glyph_size );
  placement.metrics = face.get_glyph_metrics( c );

  m_placement[ c ] = placement;

  m_next_position.x += glyph_size.x;
  m_current_line_height = std::max( m_current_line_height, glyph_size.y );
} // true_type_font::glyph_sheet::draw_character()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the sprite of a character.
 * \param character The character to get.
 */
bear::visual::sprite
bear::visual::true_type_font::glyph_sheet::get_sprite
( charset::char_type character ) const
{
  const character_to_placement::const_iterator it
    ( m_placement.find( character ) );

  if ( it == m_placement.end() )
    return sprite();
  else
    return sprite( m_image, it->second.clip );
} // true_type_font::glyph_sheet::get_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the metrics of the glyph of a character.
 * \param character The character to get.
 */
bear::visual::glyph_metrics
bear::visual::true_type_font::glyph_sheet::get_metrics
( charset::char_type character ) const
{
  const character_to_placement::const_iterator it
    ( m_placement.find( character ) );

  if ( it == m_placement.end() )
    return glyph_metrics();
  else
    return it->second.metrics;
} // true_type_font::glyph_sheet::get_metrics()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destroys all the images.
 */
void bear::visual::true_type_font::glyph_sheet::invalidate()
{
  m_image.clear();
} // true_type_font::glyph_sheet::invalidate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Builds the images of the glyphes again.
 * \param face The font_face from which we take the glyphs.
 */
void
bear::visual::true_type_font::glyph_sheet::restore( const freetype_face& face )
{
  claw::graphic::image all_glyphs( m_image.width(), m_image.height() );

  for ( character_to_placement::const_iterator it = m_placement.begin();
        it != m_placement.end(); ++it )
    all_glyphs.partial_copy
      ( face.get_glyph( it->first ), it->second.clip.position );

  m_image.restore( all_glyphs );
} // true_type_font::glyph_sheet::restore()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The font file to load.
 * \param size The size of the font.
 */
bear::visual::true_type_font::true_type_font
( const true_type_memory_file& f, double size )
  : m_font_face( f, size )
{

} // true_type_font::true_type_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the size of the font.
 */
bear::visual::size_type
bear::visual::true_type_font::get_size() const
{
  return m_font_face.get_size();
} // true_type_font::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the sprite of a character.
 * \param character The character to get.
 */
bear::visual::sprite
bear::visual::true_type_font::get_sprite( charset::char_type character )
{
  const glyph_sheet_iterator sheet( get_glyph_or_draw( character ) );

  if ( sheet == m_sheet.end() )
    return sprite();
  else
    return sheet->get_sprite( character );
} // true_type_font::get_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the metrics of the glyph of a character.
 * \param character The character to get.
 */
bear::visual::glyph_metrics
bear::visual::true_type_font::get_metrics( charset::char_type character )
{
  const glyph_sheet_iterator sheet( get_glyph_or_draw( character ) );

  if ( sheet == m_sheet.end() )
    return glyph_metrics();
  else
    return sheet->get_metrics( character );
} // true_type_font::get_metrics()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destroys all the images.
 */
void bear::visual::true_type_font::clear()
{
  for ( glyph_sheet_collection::iterator it = m_sheet.begin();
        it != m_sheet.end(); ++it )
    it->invalidate();
} // true_type_font::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Builds the images of the glyphes again.
 */
void bear::visual::true_type_font::restore()
{
  for ( glyph_sheet_collection::iterator it = m_sheet.begin();
        it != m_sheet.end(); ++it )
    it->restore( m_font_face );
} // true_type_font::restore()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds a glyph for a given character.
 * \param c The character to draw.
 * \return The glyph_sheet on which c has been drawn.
 */
bear::visual::true_type_font::glyph_sheet_iterator
bear::visual::true_type_font::draw_glyph( charset::char_type c )
{
  CLAW_PRECOND( m_sheet_from_character.find( c )
                == m_sheet_from_character.end() );

  if ( m_sheet.empty() || !m_sheet.back().can_draw( c, m_font_face ) )
    m_sheet.push_back( glyph_sheet() );

  glyph_sheet_iterator result( m_sheet.end() );
  --result;

  result->draw_character( c, m_font_face );
  m_sheet_from_character[c] = result;
  
  return result;
} // true_type_font::draw_glyph()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets an iterator on the glyph_sheet containing the glyph of a given
 *        character. If the character has never been drawn, draw_glyph() is
 *        first.
 * \param character The character for which we want the sheet.
 */
bear::visual::true_type_font::glyph_sheet_iterator
bear::visual::true_type_font::get_glyph_or_draw( charset::char_type character )
{
  glyph_sheet_iterator sheet;

  const character_to_glyph_sheet::const_iterator it =
    m_sheet_from_character.find(character);

  if ( it == m_sheet_from_character.end() )
    sheet = draw_glyph( character );
  else
    sheet = it->second;

  return sheet;
} // true_type_font::get_glyph_or_draw()
