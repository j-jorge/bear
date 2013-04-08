/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::debug::timelog class.
 * \author Julien Jorge
 */
#include "debug/timelog.hpp"

#include <fstream>
#include <algorithm>

/*----------------------------------------------------------------------------*/
std::ostream* bear::debug::timelog::s_output(NULL);

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the stream in which to write the measures.
 */
std::ostream& bear::debug::timelog::get_output()
{
  if ( s_output == NULL )
    s_output = new std::ofstream( "prof.log" );

  return *s_output;
} // timelog::get_output()

/*----------------------------------------------------------------------------*/
/**
 * \brief Formats a string to be used as the key in the timelog.
 * \param key The key to format.
 */
std::string bear::debug::timelog::format_key( std::string key )
{
  std::replace( key.begin(), key.end(), ' ', '_' );
  return key;
} // format_key()
