/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::environment_rectangle class.
 * \author Sébastien Angibaud
 */
#include "universe/environment_rectangle.hpp"

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
