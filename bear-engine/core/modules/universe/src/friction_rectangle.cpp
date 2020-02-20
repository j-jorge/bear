/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::friction_rectangle class.
 * \author Julien Jorge
 */
#include "bear/universe/friction_rectangle.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bear::universe::friction_rectangle::friction_rectangle()
{

} // friction_rectangle::friction_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param r The rectangle where the friction is different.
 * \param f The friction in this rectangle.
 */
bear::universe::friction_rectangle::friction_rectangle
( const rectangle_type& r, double f )
  : rectangle(r), friction(f)
{

} // friction_rectangle::friction_rectangle()
