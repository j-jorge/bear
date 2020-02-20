/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to represent sprites.
 * \author Julien Jorge
 */
#ifndef __VISUAL_SPRITE_HPP__
#define __VISUAL_SPRITE_HPP__

#include "bear/visual/bitmap_rendering_attributes.hpp"
#include "bear/visual/image.hpp"

#include <claw/rectangle.hpp>

#include "bear/visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief A class to represent sprites.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT sprite:
      public bitmap_rendering_attributes
    {
    public:
      /** \brief The type of the clip rectangle describing the part of the
          source image to use for the sprite.*/
      typedef claw::math::rectangle<coordinate_type> clip_rectangle_type;

    public:
      sprite();
      sprite( const image& img,
              const clip_rectangle_type& clip_rectangle );
      sprite( const image& img,
              const clip_rectangle_type& clip_rectangle,
              const rectangle_type& opaque_rectangle );
      sprite( const image& img );

      bool has_transparency() const;

      const clip_rectangle_type& clip_rectangle() const;
      void set_clip_rectangle( const clip_rectangle_type& clip );

      const rectangle_type& get_opaque_rectangle() const;
      void set_opaque_rectangle( const rectangle_type& r );

      const image& get_image() const;

      bool is_valid() const;

    private:
      /** \brief The picture where we take the sprite. */
      image m_image;

      /** \brief The sprite is this part of the image. */
      clip_rectangle_type m_clip_rectangle;

      /** \brief A part of the sprite that is fully opaque. */
      rectangle_type m_opaque_rectangle;

    }; // class sprite

  } // namespace visual
} // namespace bear

#endif // __VISUAL_SPRITE_HPP__
