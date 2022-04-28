/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The freetype_face class provides an access to font faces of the
 *        FreeType library.
 * \author Julien Jorge
 */
#ifndef __VISUAL_FREETYPE_FACE_HPP__
#define __VISUAL_FREETYPE_FACE_HPP__

#include "bear/charset/def.hpp"

#include "bear/visual/types.hpp"
#include "bear/visual/font/glyph_metrics.hpp"

#include "bear/visual/class_export.hpp"

#include <claw/non_copyable.hpp>
#include <claw/graphic/image.hpp>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace bear
{
  namespace visual
  {
    class true_type_memory_file;

    /**
     * \brief The freetype_face class provides an access to font faces of the
     *        FreeType library.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT freetype_face:
      private claw::pattern::non_copyable
    {
    public:
      freetype_face( const true_type_memory_file& f, double size );
      ~freetype_face();

      double get_size() const;

      claw::math::coordinate_2d<unsigned int>
        get_glyph_size( charset::char_type c ) const;
      glyph_metrics get_glyph_metrics( charset::char_type c ) const;

      claw::graphic::image get_glyph( charset::char_type c ) const;

    private:
      void load_char( charset::char_type c ) const;

      bool init_face( const true_type_memory_file& f );
      bool set_charmap() const;
      bool set_face_size() const;

      static void initialize_freetype();

    private:
      /** \brief The instance of the FreeType library. */
      static FT_Library s_library;

      /** \brief Tells if the FreeType library has been initialized. */
      static bool s_library_is_initialized;

      /** \brief The face of the font. */
      FT_Face m_face;

      /** \brief The last character loaded in m_face. */
      charset::char_type m_loaded_char;

      /** \brief The size of the font. */
      size_type m_size;

    }; // class freetype_face

  } // namespace visual
} // namespace bear

#endif // __VISUAL_FREETYPE_FACE_HPP__
