/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class representing a bitmap font.
 * \author Sebastien Angibaud
 */
#ifndef __VISUAL_BITMAP_FONT_HPP__
#define __VISUAL_BITMAP_FONT_HPP__

#include "visual/font/base_font.hpp"

#include "visual/class_export.hpp"

#include <map>
#include <vector>

namespace bear
{
  namespace visual
  {
    class bitmap_charmap;

    /**
     * \brief A class representing a bitmap font.
     * \author Sebastien Angibaud
     */
    class VISUAL_EXPORT bitmap_font:
      public base_font
    {
    public:
      explicit bitmap_font( const bitmap_charmap& characters );
      bitmap_font( const bitmap_charmap& characters, size_type size );

      size_type get_size() const;

      sprite get_sprite( charset::char_type character );
      glyph_metrics get_metrics( charset::char_type c );

    private:
      void make_sprites( const bitmap_charmap& characters, double size );
      void make_missing( const bitmap_charmap& characters, double size );

    private:
      /** \brief The characters in the font. */
      std::map<charset::char_type, sprite> m_characters;

      /** \brief Sprite returned for a missing character. */
      sprite m_missing;

    }; // class bitmap_font
  } // namespace visual
} // namespace bear

#endif // __VISUAL_BITMAP_FONT_HPP__
