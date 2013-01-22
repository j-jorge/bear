/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the sdc::spritedesc class.
 * \author Julien Jorge
 */
#include "spritedesc.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Finds the first with a given name.
 * \param n The name of the sprite to find.
 */
sdc::spritedesc::const_sprite_iterator
sdc::spritedesc::get_sprite_by_name( std::string n ) const
{
  for ( sprite_list::const_iterator it=sprites.begin(); it!=sprites.end();
        ++it )
    if ( it->name == n )
      return it;

  return sprites.end();
} // spritedesc::get_sprite_by_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds a sprite in description.
 * \param s The sprite to insert. Its bounds will be adjusted to the bounds of
 *        the source image. If the sprite is not visible, it wont be added in
 *        the description.
 */
void sdc::spritedesc::add_sprite( sprite s )
{
  if ( (s.source_box.width <= 0) || (s.source_box.height <= 0) )
    std::cerr << "Sprite '" << s.name << "' is not visible. Skipping."
              << std::endl;
  else
    sprites.push_back( s );
} // spritedesc::add_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns an iterator on the beginnning of the sprites.
 */
sdc::spritedesc::const_sprite_iterator sdc::spritedesc::sprite_begin() const
{
  return sprites.begin();
} // spritedesc::sprite_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns an iterator on the end of the sprites.
 */
sdc::spritedesc::const_sprite_iterator sdc::spritedesc::sprite_end() const
{
  return sprites.end();
} // spritedesc::sprite_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns an iterator on the beginnning of the sprites.
 */
sdc::spritedesc::sprite_iterator sdc::spritedesc::sprite_begin()
{
  return sprites.begin();
} // spritedesc::sprite_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns an iterator on the end of the sprites.
 */
sdc::spritedesc::sprite_iterator sdc::spritedesc::sprite_end()
{
  return sprites.end();
} // spritedesc::sprite_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Erases a sprite from the description.
 * \param it The iterator on the sprite to erase.
 */
void sdc::spritedesc::erase_sprite( sprite_iterator it )
{
  sprites.erase( it );
} // spritedesc::erase_sprite()
