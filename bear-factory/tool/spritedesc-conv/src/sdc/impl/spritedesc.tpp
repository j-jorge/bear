/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the template methods of the sdc::spritedesc class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Sorts the sprites with a given comparison function.
 * \param f The function to use to compare the sprites.
 */
template<typename F>
void sdc::spritedesc::sort_sprites( F f )
{
  sprites.sort( f );
} // spritedesc::sort_sprites()
