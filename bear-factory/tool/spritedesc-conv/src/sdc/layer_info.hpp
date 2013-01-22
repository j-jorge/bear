/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The description of a layer in the xcf file.
 * \author Julien Jorge
 */
#ifndef __SDC_LAYER_INFO_HPP__
#define __SDC_LAYER_INFO_HPP__

#include <claw/rectangle.hpp>

namespace sdc
{
  /**
   * \brief The informations of a layer.
   * \author Julien Jorge
   */
  struct layer_info
  {
    /** \brief The position and the size of the layer in the source image. */
    claw::math::rectangle<int> box;

    /** \brief The index of the layer. */
    unsigned int index;

    /** \brief */
  }; // struct layer_info

} // namespace sdc

#endif // __SDC_LAYER_INFO_HPP__
