/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Data environment_type for the universe::namespace.
 * \author Sebastien Angibaud
 */
#ifndef __UNIVERSE_ENVIRONMENT_TYPE_HPP__
#define __UNIVERSE_ENVIRONMENT_TYPE_HPP__

#include <claw/vector_2d.hpp>
#include <claw/rectangle.hpp>

namespace bear
{
  namespace universe
  {
    enum environment_type
      {
        air_environment = 0,
        fire_environment,
        ice_environment,
        water_environment
      }; // enum environment_type

  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_ENVIRONMENT_TYPE_HPP__
