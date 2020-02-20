/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An interface to the instances of base_font.
 * \author Julien Jorge
 */
#ifndef __VISUAL_FONT_HPP__
#define __VISUAL_FONT_HPP__

#include "bear/charset/def.hpp"
#include "bear/visual/sprite.hpp"
#include "bear/visual/font/glyph_metrics.hpp"

#include "bear/visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    class base_font;

    /**
     * \brief An interface to the instances of base_font.
     *
     * The instances of font share a base_font with their copies. None of them
     * will ever delete the base_font.
     *
     * \author Julien Jorge
     */
    class VISUAL_EXPORT font
    {
    public:
      /** \brief The type of the pointer to the base_font passed to the
          font. */
      typedef base_font* base_font_pointer;

    public:
      font();
      explicit font( base_font_pointer f, size_type size );

      glyph_metrics get_metrics( charset::char_type c ) const;

      size_type get_line_spacing() const;
      size_type get_size() const;

      sprite get_sprite( charset::char_type character ) const;

    private:
      /** \brief The base_font from which we take the glyphs. */
      base_font_pointer m_impl;

      /** \brief The size of the text displayed with this font. */
      size_type m_size;

    }; // class font
  } // namespace visual
} // namespace bear

#endif // __VISUAL_FONT_HPP__
