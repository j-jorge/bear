/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::force_rectangle class.
 * \author Julien Jorge
 */
#include "bear/universe/force_rectangle.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bear::universe::force_rectangle::force_rectangle()
{

} // force_rectangle::force_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param r The rectangle where the force is different.
 * \param f The force in this rectangle.
 */
bear::universe::force_rectangle::force_rectangle
( const rectangle_type& r, universe::force_type f )
  : rectangle(r), force(f)
{

} // force_rectangle::force_rectangle()
