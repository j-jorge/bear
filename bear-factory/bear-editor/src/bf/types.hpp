/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The various types used in the engine.
 * \author Julien Jorge
 */
#ifndef __BF_TYPES_HPP__
#define __BF_TYPES_HPP__

#include <claw/box_2d.hpp>
#include <claw/coordinate_2d.hpp>

namespace bf
{
  /** \brief The type of the coordinates. */
  typedef double coordinate_type;

  /** \brief The type of a point. */
  typedef claw::math::coordinate_2d<coordinate_type> position_type;

  /** \brief The type of the dimensions of a box. */
  typedef claw::math::coordinate_2d<coordinate_type> size_box_type;

  /** \brief The type of the rectangles. */
  typedef claw::math::box_2d<coordinate_type> rectangle_type;

} // namespace bf

#endif // __BF_TYPES_HPP__
