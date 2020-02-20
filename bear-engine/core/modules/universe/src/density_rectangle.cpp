/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::density_rectangle class.
 * \author Sebastien Angibaud
 */
#include "bear/universe/density_rectangle.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bear::universe::density_rectangle::density_rectangle()
{

} // density_rectangle::density_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param r The rectangle where the density is different.
 * \param f The density in this rectangle.
 */
bear::universe::density_rectangle::density_rectangle
( const rectangle_type& r, double f )
  : rectangle(r), density(f)
{

} // density_rectangle::density_rectangle()
