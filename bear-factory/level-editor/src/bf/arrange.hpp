/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Functions to transform a collection of coordinates into equidistant
 *        coordinates.
 * \author Julien Jorge
 */
#ifndef __BF_ARRANGE_HPP__
#define __BF_ARRANGE_HPP__

#include <set>
#include <map>

namespace bf
{
  std::map<double, double> arrange( const std::set<double>& s );

} // namespace bf

#endif // __BF_ARRANGE_HPP__
