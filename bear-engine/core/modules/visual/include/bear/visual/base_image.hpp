/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Base class for all image implementation.
 * \author Julien Jorge
 */
#ifndef __VISUAL_BASE_IMAGE_HPP__
#define __VISUAL_BASE_IMAGE_HPP__

#include <claw/coordinate_2d.hpp>
#include <claw/graphic/image.hpp>
#include <claw/non_copyable.hpp>

#include "bear/visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief Base class for all image implementation.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT base_image:
      public claw::pattern::non_copyable
    {
    public:
      virtual ~base_image() {}

      unsigned int width() const { return size().x; }
      unsigned int height() const { return size().y; }
      virtual claw::math::coordinate_2d<unsigned int> size() const = 0;
      virtual bool has_transparency() const = 0;

      virtual void draw
        ( const claw::graphic::image& data,
          claw::math::coordinate_2d<unsigned int> pos ) = 0;

      virtual claw::graphic::image read() const = 0;

    }; // class base_image
  } // namespace visual
} // namespace bear

#endif // __VISUAL_BASE_IMAGE_HPP__
