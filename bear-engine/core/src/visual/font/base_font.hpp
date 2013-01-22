/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The common interface to the classes representing the fonts.
 * \author Julien Jorge
 */
#ifndef __VISUAL_BASE_FONT_HPP__
#define __VISUAL_BASE_FONT_HPP__

#include "charset/def.hpp"
#include "visual/sprite.hpp"
#include "visual/font/glyph_metrics.hpp"

#include "visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief The common interface to the classes representing the fonts.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT base_font
    {
    public:
      virtual ~base_font();

      virtual size_type get_size() const = 0;

      virtual glyph_metrics get_metrics( charset::char_type c ) = 0;
      virtual sprite get_sprite( charset::char_type character ) = 0;

      virtual void clear();
      virtual void restore();

    }; // class base_font
  } // namespace visual
} // namespace bear

#endif // __VISUAL_BASE_FONT_HPP__
