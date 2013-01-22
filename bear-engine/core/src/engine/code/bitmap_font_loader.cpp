/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bitmap_font_loader class.
 * \author Julien Jorge
 */
#include "engine/bitmap_font_loader.hpp"

#include "engine/level_globals.hpp"
#include "engine/resource_pool.hpp"
#include "engine/spritepos.hpp"

#include "visual/font/bitmap_font.hpp"

#include <claw/exception.hpp>

#include <limits>

/*----------------------------------------------------------------------------*/
const std::string
bear::engine::bitmap_font_loader::s_autofont_keyword( "autofont" );
const std::string
bear::engine::bitmap_font_loader::s_options_keyword( "options" );

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The file from which we load the file.
 * \param glob The level_globals in which we load the resources.
 */
bear::engine::bitmap_font_loader::bitmap_font_loader
( std::istream& f, level_globals& glob )
  : m_file(f), m_level_globals(glob)
{

} // bitmap_font_loader::bitmap_font_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads the font.
 */
bear::visual::bitmap_charmap bear::engine::bitmap_font_loader::run()
{
  const std::istream::pos_type p( m_file.tellg() );

  const std::string line( get_next_line() );
  m_file.seekg( p );

  if ( line == s_autofont_keyword )
    return read_autofont();
  else
    return read_legacy();
} // bitmap_font_loader::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the font in the legacy format.
 *
 * The legacy format is defined as follows:
 *
 * The file begins with two values giving the width and the height of the
 * characters. They are followed by the number of images in which we take
 * the characters, followed by the image resources.
 *
 * The remaining of the file consists in a description of the
 * characters. Each line has four fields. The first one is the character,
 * then come the X and Y position of the character in the image. The last
 * field is the zero-based index of the image resource where the
 * character is, in the list defined before.
 *
 * There must be no empty lines between the image resources and the lines
 * describing the characters.
 */
bear::visual::bitmap_charmap bear::engine::bitmap_font_loader::read_legacy()
{
  visual::bitmap_charmap cs;
  unsigned int n = 0;

  std::istringstream parser(get_next_line());

  parser >> cs.size.x >> cs.size.y;

  parser.clear();
  parser.str( get_next_line() );

  if( parser >> n )
    cs.font_images.reserve(n);

  for ( std::size_t i=0; i!=n; ++i )
    cs.font_images.push_back( m_level_globals.get_image(get_next_line()) );

  std::string line = get_next_line();
  while ( !line.empty() )
    {
      unsigned int x, y;
      std::size_t i;
      char c;
      parser.clear();
      parser.str(line);

      if ( (parser.get(c) >> x >> y >> i) && (i < n) )
        {
          cs.characters[c].image_index = i;
          cs.characters[c].position.set(x, y);
        }

      line = get_next_line();
    }

  if ( !( (cs.font_images.size() == n) && m_file.eof()) )
    fail( "Bad font" );

  return cs;
} // bitmap_font_loader::read_legacy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the font in the autofont format.
 *
 * The format is defined as follows:
 *
 * The file begins with the "autofont" keyword. Then the images from which the
 * caracters are taken are listed, with one file per line. The font is built
 * from the .spritepos files associated with the images. For each entry whose
 * name is a single caracter, the corrisponding sprite is assigned to the
 * character in the font.
 *
 * The options of the font can be set at the end of the font file with the
 * "options" keyword. The single available option is "upper_to_lower", which
 * builds the glyphes for the lower case letters by assigning the ones of the
 * corresponding upper case letter.
 */
bear::visual::bitmap_charmap bear::engine::bitmap_font_loader::read_autofont()
{
  visual::bitmap_charmap cs;

  std::string line(get_next_line());

  if ( line != s_autofont_keyword )
    fail( "Missing keyword: " + s_autofont_keyword );

  line = get_next_line();

  while ( !line.empty() && (line != s_options_keyword) )
    {
      read_autofont_image(cs, line);
      line = get_next_line();
    }

  if ( line == s_options_keyword )
    read_autofont_options( cs );

  return cs;
} // bitmap_font_loader::read_autofont()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the glyphes of the characters from the spritepos file associated
 * with a given image.
 * \param image_name The name of the image from which the sprites are taken.
 * \param cs The symbol table where the characters are saved.
 */
void bear::engine::bitmap_font_loader::read_autofont_image
( visual::bitmap_charmap& cs, const std::string& image_name ) const
{
  const std::string spritepos_path
    ( m_level_globals.get_spritepos_path( image_name ) );

  if ( spritepos_path.empty() )
    fail( "No spritepos file for \"" + image_name + '"' );

  std::stringstream f;
  resource_pool::get_instance().get_file(spritepos_path, f);
  const spritepos pos( f );

  const std::size_t image_index( cs.font_images.size() );
  cs.font_images.push_back( m_level_globals.get_image(image_name) );

  for ( spritepos::const_iterator it=pos.begin(); it!=pos.end(); ++it )
    if ( it->get_name().length() == 1 )
      {
        const char c( it->get_name()[0] );

        cs.characters[c].image_index = image_index;
        cs.characters[c].position.set
          ( it->get_clip().left(), it->get_clip().top() );
        
        cs.size = it->get_clip().size();
      }
} // bitmap_font_loader::read_autofont_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the options of the autofont file.
 * \param cs The symbol table where the characters are saved.
 */
void bear::engine::bitmap_font_loader::read_autofont_options
( visual::bitmap_charmap& cs )
{
  std::string line( get_next_line() );

  while ( !line.empty() )
    {
      if ( line == "upper_to_lower" )
        upper_to_lower(cs);
      else
        fail( "Unknown option: \"" + line + '"' );

      line = get_next_line();
    }
} // bitmap_font_loader::read_autofont_options()

/*----------------------------------------------------------------------------*/
/**
 * \brief Builds the glyphes for the lower case characters from their upper case
 *        counterpart.
 * \param cs The symbol table where the characters are saved.
 */
void bear::engine::bitmap_font_loader::upper_to_lower
( visual::bitmap_charmap& cs ) const
{
  visual::bitmap_charmap::character_map new_characters;

  typedef
    visual::bitmap_charmap::character_map::const_iterator
    character_map_iterator;

  for ( character_map_iterator it=cs.characters.begin();
        it!=cs.characters.end();
        ++it )
    {
      charset::char_type c( std::tolower( it->first ) );

      if ( cs.characters.find(c) == cs.characters.end() )
        new_characters[c] = it->second;
    }

  cs.characters.insert( new_characters.begin(), new_characters.end() );
} // bitmap_font_loader::upper_to_lower()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the next non empty line from the file, if any.
 */
std::string bear::engine::bitmap_font_loader::get_next_line() const
{
  std::string result;

  while ( result.empty() && getline(m_file, result) );

  return result;
} // bitmap_font_loader::get_next_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Process an error in the processing of the font file.
 * \param msg The message describing the error.
 */
void bear::engine::bitmap_font_loader::fail( const std::string& msg ) const
{
  throw claw::exception(msg);
} // bitmap_font_loader::fail()
