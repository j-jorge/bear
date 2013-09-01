/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Data types for the input:: namespace.
 * \author Julien Jorge
 */
#ifndef __INPUT_TYPES_HPP__
#define __INPUT_TYPES_HPP__

#include <claw/coordinate_2d.hpp>

namespace bear
{
  namespace input
  {
    /** \brief The type of a coordinate. */
    typedef int coordinate_type;

    /** \brief The type of the vectors used for representing coordinates. */
    typedef claw::math::coordinate_2d<coordinate_type> position_type;

  } // namespace input
} // namespace bear

#endif // __INPUT_TYPES_HPP__
