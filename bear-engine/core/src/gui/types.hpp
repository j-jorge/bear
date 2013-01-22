/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Data types for the gui:: namespace.
 * \author Julien Jorge
 */
#ifndef __GUI_TYPES_HPP__
#define __GUI_TYPES_HPP__

#include "visual/types.hpp"

namespace bear
{
  namespace gui
  {
    /** \brief The type of a coordinate. */
    typedef visual::coordinate_type coordinate_type;

    /** \brief The type of the vectors used for representing coordinates. */
    typedef visual::position_type position_type;

    /** \brief The type of a size. */
    typedef visual::size_type size_type;

    /** \brief The type of a box size (width and height). */
    typedef visual::size_box_type size_box_type;

    /** \brief The type of a rectangle. */
    typedef visual::rectangle_type rectangle_type;

    /** \brief A color. */
    typedef visual::color_type color_type;

  } // namespace gui
} // namespace bear

#endif // __GUI_TYPES_HPP__
