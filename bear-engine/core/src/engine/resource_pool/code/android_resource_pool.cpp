/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::android_resource_pool class.
 * \author Julien Jorge
 */
#ifdef __ANDROID__

#include "engine/resource_pool/android_resource_pool.hpp"

#include <fstream>
#include <claw/exception.hpp>
#include <claw/logger.hpp>

#include <SDL/SDL.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a file.
 * \param name The path of the file to get.
 * \param os Where we must write the content of the file.
 */
void bear::engine::android_resource_pool::get_file
( const std::string& name, std::ostream& os )
{
  SDL_RWops* rw = SDL_RWFromFile( name.c_str(), "rb" );

  if ( rw == NULL )
    throw claw::exception( "Can't find file '" + name + "'" );

  const std::size_t buffer_size(4096);
  char buffer[buffer_size];

  int count = SDL_RWread( rw, buffer, sizeof(char), buffer_size );

  while ( count > 0 )
    {
      os.write( buffer, count );
      count = SDL_RWread( rw, buffer, sizeof(char), buffer_size );
    }

  SDL_RWclose( rw );
} // android_resource_pool::get_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Checks if we know a file with a given name.
 * \param name The name of the file to find.
 */
bool
bear::engine::android_resource_pool::exists( const std::string& name ) const
{
  SDL_RWops* rw = SDL_RWFromFile( name.c_str(), "rb" );

  if ( rw == NULL )
    return false;

  SDL_RWclose(rw);

  return true;
} // android_resource_pool::exists()

#endif // __ANDROID__
