/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::freetype_face class.
 * \author Julien Jorge
 */
#include "visual/font/freetype_face.hpp"

#include "visual/font/true_type_memory_file.hpp"

#include <claw/assert.hpp>
#include <claw/exception.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
FT_Library bear::visual::freetype_face::s_library;
bool bear::visual::freetype_face::s_library_is_initialized(false);

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The font file to load.
 * \param size The size of the font.
 */
bear::visual::freetype_face::freetype_face
( const true_type_memory_file& f, size_type size )
  : m_face(NULL), m_loaded_char(0), m_size(size)
{
  initialize_freetype();

  if ( !init_face( f ) )
    throw claw::exception( "Could not load the font." );
} // freetype_face::freetype_face()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destroys the font.
 */
bear::visual::freetype_face::~freetype_face()
{
  const FT_Error error = FT_Done_Face( m_face );

  if ( error )
    claw::logger << claw::log_error
                 << "Error when releasing the font. Error is "
                 << error << "." << std::endl;
} // freetype_face::~freetype_face()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the size of the font face.
 */
bear::visual::size_type bear::visual::freetype_face::get_size() const
{
  return m_size;
} // freetype_face::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Computes the size of a glyph.
 * \param c The glyph from which we take the size.
 */
claw::math::coordinate_2d<unsigned int>
bear::visual::freetype_face::get_glyph_size( charset::char_type c ) const
{
  load_char( c );
  
  return claw::math::coordinate_2d<unsigned int>
    ( m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows );
} // freetype_face::get_glyph_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Computes the metrics of a glyph.
 * \param c The glyph from which we take the metrics.
 */
bear::visual::glyph_metrics
bear::visual::freetype_face::get_glyph_metrics( charset::char_type c ) const
{
  const size_type glyph_height( get_glyph_size(c).y );
  const size_type advance_units_per_pixel(64);

  const size_box_type advance
    ( size_box_type( m_face->glyph->advance.x, m_face->glyph->advance.y )
      / advance_units_per_pixel );
  const size_box_type bearing
    ( m_face->glyph->bitmap_left, m_face->glyph->bitmap_top - glyph_height );

  return glyph_metrics( advance, bearing );
} // freetype_face::get_glyph_metrics()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the bitmap of a glyph.
 * \param c The character from which we want the glyph.
 */
claw::graphic::image
bear::visual::freetype_face::get_glyph( charset::char_type c ) const
{
  const claw::math::coordinate_2d<unsigned int> glyph_size( get_glyph_size(c) );
  claw::graphic::image img( glyph_size.x, glyph_size.y );

  FT_GlyphSlot glyph = m_face->glyph;
  unsigned char* buffer = glyph->bitmap.buffer;

  for ( unsigned int py=0; py != img.height(); ++py )
    for ( unsigned int px=0; px != img.width(); ++px )
      {
        img[py][px] = claw::graphic::white_pixel;
        img[py][px].components.alpha = *buffer;
        ++buffer;
      }

  return img;
} // freetype_face::get_glyph()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads the glyph of a given character.
 * \param c The character to draw.
 */
void bear::visual::freetype_face::load_char( charset::char_type c ) const
{
  if ( c == m_loaded_char )
    return;

  const FT_Error error =
    FT_Load_Char( m_face, (unsigned char)c, FT_LOAD_RENDER );

  if ( error )
    {
      claw::logger << claw::log_error << "Error loading glyph '" << c
                   << "'. Error is " << error << "." << std::endl;

      const_cast<freetype_face*>(this)->m_loaded_char = 0;
    }
  else
    const_cast<freetype_face*>(this)->m_loaded_char = c;
} // freetype_face::load_char()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initializes the font face with a given file and size.
 * \param f The font file to load.
 * \return true on success, false otherwise.
 */
bool bear::visual::freetype_face::init_face( const true_type_memory_file& f )
{
  const FT_Error error =
    FT_New_Memory_Face( s_library, f.begin(), f.end() - f.begin(), 0, &m_face );

  if ( error )
    {
      claw::logger << claw::log_error
                   << "Could not load the font. Error is " << error << "."
                   << std::endl;
      return false;
    }
  else
    return set_face_size() && set_charmap();
} // freetype_face::init_face()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initializes the charmap of the font.
 * \return true on success, false otherwise.
 */
bool bear::visual::freetype_face::set_charmap() const
{
  const FT_Error error = FT_Select_Charmap( m_face, FT_ENCODING_UNICODE );

  if ( error )
    {
      claw::logger << claw::log_error
                   << "Could not set the charmap. Error is "
                   << error << "." << std::endl;
      return false;
    }
  else
    return true;
} // freetype_face::set_charmap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the size of the font.
 * \return true on success, false otherwise.
 */
bool bear::visual::freetype_face::set_face_size() const
{
  const FT_Error error =
    FT_Set_Pixel_Sizes( m_face, 0, (unsigned int)m_size );

  if ( error )
    {
      claw::logger << claw::log_error
                   << "Could not set the font's size. Error is "
                   << error << "." << std::endl;
      return false;
    }
  else
    return true;
} // freetype_face::set_face_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initializes the FreeType library.
 */
void bear::visual::freetype_face::initialize_freetype()
{
  if ( s_library_is_initialized )
    return;

  const FT_Error error = FT_Init_FreeType( &s_library );

  if ( error )
    claw::logger << claw::log_error
                 << "Could not initialize FreeType. Error is " << error << "."
                 << std::endl;
  else
    s_library_is_initialized = true;
} // freetype_face::initialize_freetype()
