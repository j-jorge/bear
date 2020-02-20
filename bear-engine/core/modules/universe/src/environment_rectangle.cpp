/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::environment_rectangle class.
 * \author Sébastien Angibaud
 */
#include "bear/universe/environment_rectangle.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bear::universe::environment_rectangle::environment_rectangle()
{

} // environment_rectangle::environment_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param r The rectangle where the environment is different.
 * \param e The environment in this rectangle.
 */
bear::universe::environment_rectangle::environment_rectangle
( const rectangle_type& r, const environment_type e )
  : rectangle(r), environment(e)
{

} // environment_rectangle::environment_rectangle()
