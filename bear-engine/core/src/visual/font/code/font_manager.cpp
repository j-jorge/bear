/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the visual::font_manager class.
 * \author Julien Jorge
 */
#include "visual/font/font_manager.hpp"

#include "visual/font/base_font.hpp"
#include "visual/font/bitmap_charmap.hpp"
#include "visual/font/bitmap_font.hpp"
#include "visual/font/true_type_font.hpp"
#include "visual/font/true_type_memory_file.hpp"

#include <boost/tuple/tuple_comparison.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::visual::font_manager::~font_manager()
{
  clear();
} // font_manager::~font_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Deletes all the fonts.
 */
void bear::visual::font_manager::clear()
{
  for ( font_map_type::iterator it = m_font.begin(); it != m_font.end(); ++it )
    delete it->second;

  m_font.clear();
  m_memory_file.clear();
  m_bitmap_charmap.clear();
} // font_manager::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the mapping of the characters for a bitmap font.
 * \param name The identifier of the font.
 * \param m The character mapping to associate with name.
 * \pre !exists( name )
 */
void bear::visual::font_manager::load_font
( std::string name, const bitmap_charmap& m )
{
  CLAW_PRECOND( !exists(name) );

  m_bitmap_charmap[ name ] = m;
} // font_manager::load_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the data of a true type font file.
 * \param name The identifier of the font.
 * \param file The content of the file.
 * \pre !exists( name )
 */
void bear::visual::font_manager::load_font
( std::string name, std::istream& file )
{
  CLAW_PRECOND( !exists(name) );

  m_memory_file.insert( std::make_pair( name, true_type_memory_file( file ) ) );
} // font_manager::load_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clears the fonts for future restoration. The fonts are not deleted.
 */
void bear::visual::font_manager::clear_fonts()
{
  for ( font_map_type::iterator it = m_font.begin(); it != m_font.end(); ++it )
    it->second->clear();
} // font_manager::clear_fonts()

/*----------------------------------------------------------------------------*/
/**
 * \brief Restore all the fonts.
 */
void bear::visual::font_manager::restore_fonts()
{
  for ( font_map_type::iterator it = m_font.begin(); it != m_font.end(); ++it )
    it->second->restore();
} // font_manager::restore_fonts()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns a font of a given size from an existing font.
 * \param name The name of the font to find.
 * \param size The size of the font.
 */
bear::visual::font
bear::visual::font_manager::get_font( std::string name, double size ) const
{
  CLAW_PRECOND( exists(name) );

  const font_id_type font_id( boost::make_tuple( name, size ) );
  const font_map_type::iterator it( m_font.find( font_id ) );

  if ( it != m_font.end() )
    return font( it->second, size );
  else
    {
      const id_to_charmap_type::const_iterator charmap_it
        ( m_bitmap_charmap.find( name ) );

      if ( charmap_it != m_bitmap_charmap.end() )
        m_font[ font_id ] = new bitmap_font( charmap_it->second, size );
      else
        m_font[ font_id ] =
          new true_type_font( m_memory_file.find( name )->second, size );

      return font( m_font[ font_id ], size );
    }
} // font_manager::get_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if there is a font with a given name.
 * \param name The name of the font to search.
 */
bool bear::visual::font_manager::exists( const std::string& name ) const
{
  return ( m_bitmap_charmap.find( name ) != m_bitmap_charmap.end() )
    || ( m_memory_file.find( name ) != m_memory_file.end() );
} // font_manager::exists()
