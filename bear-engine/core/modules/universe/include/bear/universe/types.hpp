/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Data types for the universe:: namespace.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_TYPES_HPP__
#define __UNIVERSE_TYPES_HPP__

#include <claw/vector_2d.hpp>
#include <claw/rectangle.hpp>

namespace bear
{
  namespace universe
  {
    /** \brief The type of the vectors. */
    typedef claw::math::vector_2d<double> vector_type;

    /** \brief The type of the vectors used for representing forces. */
    typedef vector_type force_type;

    /** \brief The type of the vectors used for representing speed. */
    typedef vector_type speed_type;

    /** \brief The type of a coordinate. */
    typedef double coordinate_type;

    /** \brief The type of the vectors used for representing coordinates. */
    typedef claw::math::coordinate_2d<coordinate_type> position_type;

    /** \brief The type of the rectangles. */
    typedef claw::math::box_2d<coordinate_type> rectangle_type;

    /** \brief The type of a size. */
    typedef double size_type;

    /** \brief The type of a box size (width and height). */
    typedef claw::math::coordinate_2d<size_type> size_box_type;

    /** \brief Type used for representing time. */
    typedef double time_type;

  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_TYPES_HPP__
