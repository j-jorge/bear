/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::spritepos class.
 * \author Julien Jorge
 */
#include "engine/spritepos.hpp"

#include <claw/string_algorithm.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Build a spritepos entry given its name and clip rectangle.
 * \param name The name of the entry.
 * \param clip The region of the sprite in the source image.
 */
bear::engine::spritepos::sprite_entry::sprite_entry
( const std::string& name, const rectangle_type& clip )
  : m_name(name), m_clip(clip)
{

} // sprite_entry::sprite_entry()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the entry.
 */
const std::string& bear::engine::spritepos::sprite_entry::get_name() const
{
  return m_name;
} // sprite_entry::get_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the region of the sprite in the source image.
 */
const bear::engine::spritepos::sprite_entry::rectangle_type&
bear::engine::spritepos::sprite_entry::get_clip() const
{
  return m_clip;
} // sprite_entry::get_clip()




/*----------------------------------------------------------------------------*/
/**
 * \brief Build a spritepos by reading the entries from a stream.
 * \param f The stream from which we load the entries.
 */
bear::engine::spritepos::spritepos( std::istream& f )
{
  load(f);
} // spritepos::spritepos()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first entry.
 */
bear::engine::spritepos::const_iterator bear::engine::spritepos::begin() const
{
  return m_entries.begin();
} // spritepos::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator just past the last entry.
 */
bear::engine::spritepos::const_iterator bear::engine::spritepos::end() const
{
  return m_entries.end();
} // spritepos::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first entry with a given name.
 * \param name The name of the entry.
 */
bear::engine::spritepos::const_iterator
bear::engine::spritepos::find( const std::string& name ) const
{
  for ( const_iterator it=m_entries.begin(); it!=m_entries.end(); ++it )
    if ( it->get_name() == name )
      return it;

  return end();
} // spritepos::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a spritepos file.
 * \param f The file from which we load the entries.
 */
void bear::engine::spritepos::load( std::istream& f )
{
  std::string line;

  while ( claw::text::getline(f, line) )
    if ( !line.empty() )
      if ( line[0] != '#' )
        {
          std::string::size_type pos( line.find_first_of(':') );
          const std::string::size_type next
            ( line.find_first_of( ':', pos + 1 ) );

          if ( next != std::string::npos )
            pos = next;

          std::string n( line.substr(0, pos) );
          claw::text::trim(n);

          // read the coordinates of the clip rectangle
          std::istringstream iss( line.substr(pos+1) );
          
          sprite_entry::rectangle_type r;

          if ( iss >> r.position.x >> r.position.y >> r.width >> r.height )
            m_entries.push_back( sprite_entry( n, r ) );
        }
} // spritepos::load()
