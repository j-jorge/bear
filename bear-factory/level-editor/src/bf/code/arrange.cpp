/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the functions of the arrange.hpp file.
 * \author Julien Jorge
 */
#include "bf/arrange.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Associates a new position to some coordinates in order to build a list
 *        of positions where the distance between two adjacent coordinates stays
 *        equal in the whole list.
 * \param s The positions to arrange.
 */
std::map<double, double> bf::arrange( const std::set<double>& s )
{
  std::map<double, double> result;

  if ( s.size() < 3 )
    return result;

  const std::set<double>::const_iterator first( s.begin() );
  const std::set<double>::const_iterator last( --s.end() );
  
  result[ *first ] = *first;
  result[ *last ] = *last;

  std::size_t i(1);
  std::set<double>::const_iterator it( first );
  
  for ( ++it; it!=last; ++it, ++i )
    result[ *it ] = *first + i * (*last - *first) / (s.size() - 1);

  return result;
} // arrange()

