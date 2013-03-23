/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An image class, used for sprites.
 * \author Julien Jorge
 */
#ifndef __VISUAL_IMAGE_HPP__
#define __VISUAL_IMAGE_HPP__

#include <string>

#include <claw/pixel.hpp>
#include <claw/image.hpp>
#include <claw/smart_ptr.hpp>

#include "visual/base_image.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief An image class, used for sprites.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT image
    {
    private:
      typedef claw::memory::smart_ptr<base_image> base_image_ptr;

    public:
      image();
      image( unsigned int width, unsigned int height );
      explicit image( const claw::graphic::image& data );

      void clear();
      void restore( const claw::graphic::image& data );

      unsigned int width() const;
      unsigned int height() const;
      claw::math::coordinate_2d<unsigned int> size() const;
      bool has_transparency() const;
      bool is_valid() const;

      void draw
        ( const claw::graphic::image& data,
          claw::math::coordinate_2d<unsigned int> pos );

      const base_image* get_impl() const;

    private:
      /**
       * \brief The implementation of the image.
       *
       * The implementation is shared among the instances, thus if one of them
       * is restored or replaced then all instances of image becomes clean.
       */
      claw::memory::smart_ptr<base_image_ptr> m_impl;

    }; // class image
  } // namespace visual
} // namespace bear

#endif // __VISUAL_IMAGE_HPP__
