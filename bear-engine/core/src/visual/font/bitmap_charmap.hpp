/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The bitmap_charmap maps the positions of a set of characters into
 *        the region representing their glyphs in a collection of images.
 * \author Julien Jorge
 */
#ifndef __VISUAL_BITMAP_CHARMAP_HPP__
#define __VISUAL_BITMAP_CHARMAP_HPP__

#include "visual/image.hpp"

#include "charset/def.hpp"

#include "visual/class_export.hpp"

#include <claw/coordinate_2d.hpp>

#include <map>
#include <vector>

namespace bear
{
  namespace visual
  {
    /**
     * \brief The bitmap_charmap maps the positions of a set of characters into
     *        the region representing their glyphs in a collection of images.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT bitmap_charmap
    {
    public:
      /** \brief The position of a character. */
      struct char_position
      {
      public:
        /** \brief The index of the image. */
        unsigned int image_index;

        /** \brief The position of the character in the image. */
        claw::math::coordinate_2d<unsigned int> position;

      }; // struct char_position

      /** \brief The type of the map in which the descriptions of the
          characters are stored. */
      typedef std::map<charset::char_type, char_position> character_map;

    public:
      bitmap_charmap();

    public:
      /** \brief The size of the characters. */
      claw::math::coordinate_2d<unsigned int> size;

      /** \brief The images in which we take the images of the characters. */
      std::vector<visual::image> font_images;

      /** \brief The position of the characters in the image. */
      character_map characters;

    }; // class bitmap_charmap

  } // namespace visual
} // namespace bear

#endif // __VISUAL_BITMAP_CHARMAP_HPP__
