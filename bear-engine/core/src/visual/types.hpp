/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Data types for the visual:: namespace.
 * \author Julien Jorge
 */
#ifndef __VISUAL_TYPES_HPP__
#define __VISUAL_TYPES_HPP__

#include <claw/coordinate_2d.hpp>
#include <claw/box_2d.hpp>
#include "visual/color.hpp"

namespace bear
{
  namespace visual
  {
    /** \brief The type of a coordinate. */
    typedef double coordinate_type;

    /** \brief The type of the vectors used for representing coordinates. */
    typedef claw::math::coordinate_2d<coordinate_type> position_type;

    /** \brief The type of a size. */
    typedef double size_type;

    /** \brief The type of a box size (width and height). */
    typedef claw::math::coordinate_2d<size_type> size_box_type;

    /** \brief The type of a rectangle. */
    typedef claw::math::box_2d<coordinate_type> rectangle_type;

    /** \brief A color. */
    typedef color color_type;

  } // namespace visual
} // namespace bear

#endif // __VISUAL_TYPES_HPP__
