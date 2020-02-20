/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::util class.
 * \author Julien Jorge
 */
#include "bf/xml/util.hpp"

#include "boost/algorithm/string/replace.hpp"

/*----------------------------------------------------------------------------*/
bf::xml::util::replacement_map_type bf::xml::util::s_replacement_map;

/*----------------------------------------------------------------------------*/
/**
 * \brief Replace special XML characters with adequate entities.
 * \param str The string to process.
 */
std::string bf::xml::util::replace_special_characters( const std::string& str )
{
  if ( s_replacement_map.empty() )
    build_replacement_map();

  std::string result;

  result.reserve( str.size() );
  
  for ( std::size_t i(0); i!=str.size(); ++i )
    {
      const replacement_map_type::const_iterator it
        ( s_replacement_map.find( str[i] ) );

      if ( it == s_replacement_map.end() )
        result.push_back( str[i] );
      else
        result.append( it->second );
    }

  return result;
} // util::replace_special_characters()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fills s_replacement_map with the adequate replacements.
 */
void bf::xml::util::build_replacement_map()
{
  s_replacement_map[ '\'' ] = "&apos;";
  s_replacement_map[ '"' ] = "&quot;";
  s_replacement_map[ '<' ] = "&lt;";
  s_replacement_map[ '>' ] = "&gt;";
  s_replacement_map[ '&' ] = "&amp;";

  s_replacement_map[ 'à' ] = "&#224;";
  s_replacement_map[ 'ä' ] = "&#228;";
  s_replacement_map[ 'â' ] = "&#226;";

  s_replacement_map[ 'é' ] = "&#233;";
  s_replacement_map[ 'è' ] = "&#232;";
  s_replacement_map[ 'ë' ] = "&#235;";
  s_replacement_map[ 'ê' ] = "&#234;";

  s_replacement_map[ 'ï' ] = "&#239;";
  s_replacement_map[ 'î' ] = "&#238;";

  s_replacement_map[ 'ö' ] = "&#246;";
  s_replacement_map[ 'ô' ] = "&#244;";

  s_replacement_map[ 'ù' ] = "&#249;";
  s_replacement_map[ 'ü' ] = "&#252;";
  s_replacement_map[ 'û' ] = "&#251;";
} // util::build_replacement_map()
