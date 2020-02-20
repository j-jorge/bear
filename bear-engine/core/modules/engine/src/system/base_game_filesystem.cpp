/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::base_game_filesystem class.
 * \author Julien Jorge
 */
#include "bear/engine/system/base_game_filesystem.hpp"

#include <claw/string_algorithm.hpp>
#include <algorithm>

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::base_game_filesystem::~base_game_filesystem()
{
  // Nothing to do.
} // base_game_filesystem::~base_game_filesystem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Converts a string in order to create a valid name file, with no
 *        blanks.
 * \param name The name to convert.
 */
std::string
bear::engine::base_game_filesystem::get_name_as_filename
( std::string name ) const
{
  std::string result( name );
  std::transform( result.begin(), result.end(), result.begin(), tolower );

  const std::string prohibited( " \t<>:\"|/\\?*" );

  for (unsigned int i=0; i!=result.size(); ++i)
    if ( prohibited.find_first_of( result[i] ) != std::string::npos )
      result[i] = '_';

  claw::text::squeeze( result, "_" );

  return result;
} // game_local_client::get_game_name_as_filename()
