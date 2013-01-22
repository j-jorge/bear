/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class representing a true type font.
 * \author Julien Jorge
 */
#ifndef __VISUAL_TRUE_TYPE_FONT_HPP__
#define __VISUAL_TRUE_TYPE_FONT_HPP__

#include "visual/font/base_font.hpp"
#include "visual/font/freetype_face.hpp"

#include "visual/class_export.hpp"

#include <map>
#include <list>

namespace bear
{
  namespace visual
  {
    class true_type_memory_file;

    /**
     * \brief A class representing a true type font.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT true_type_font:
      public base_font
    {
    private:
      /**
       * \brief A glyph sheet is an image representing some glyphs. New glyphs
       *        can be drawn on the sheets.
       */
      class glyph_sheet
      {
      private:
        /** \brief The type of the positions in the image. */
        typedef claw::math::coordinate_2d<unsigned int> position_type;

        /** \brief The type of the size of a box. */
        typedef claw::math::coordinate_2d<unsigned int> size_box_type;

        /** \brief The type of a region containing a glyph in the image. */
        typedef claw::math::rectangle<unsigned int> clip_rectangle;

        /**
         * \brief The character_placement structures describes the informations
         *        related to the glyph of a character.
         */
        struct character_placement
        {
          /** \brief The region containing the glyph in the image. */
          clip_rectangle clip;

          /** \brief The metrics of the glyph. */
          glyph_metrics metrics;

        }; // struct character_placement

        /** \brief The type of the map associating a character, as the key, with
            its placement, as the value. */
        typedef std::map<charset::char_type, character_placement>
        character_to_placement;

      public:
        glyph_sheet();

        bool can_draw( charset::char_type c, const freetype_face& face ) const;

        void draw_character
        ( charset::char_type c, const freetype_face& face );

        sprite get_sprite( charset::char_type character ) const;
        glyph_metrics get_metrics( charset::char_type character ) const;

        void invalidate();
        void restore( const freetype_face& face );

      private:
        /** \brief The image where the glyphs are drawn. */
        image m_image;

        /** \brief The position where the next glyph will be drawn. */
        position_type m_next_position;

        /** \brief The height of the current line of glyphs. */
        unsigned int m_current_line_height;

        /** \brief The placement of each character. */
        character_to_placement m_placement;

      }; // class glyph_sheet
      
      /** \brief The type of the collection in which the glyph_sheets are
          stored. */
      typedef std::list<glyph_sheet> glyph_sheet_collection;

      /** \brief The type of an iterator in a glyph_sheet_collection. */
      typedef glyph_sheet_collection::iterator glyph_sheet_iterator;

      /** \brief The type of the map associating a character with the glyph
          sheet in which its glyph was created. */
      typedef
        std::map<charset::char_type, glyph_sheet_iterator>
        character_to_glyph_sheet;

    public:
      true_type_font( const true_type_memory_file& f, double size );

      size_type get_size() const;

      sprite get_sprite( charset::char_type character );
      glyph_metrics get_metrics( charset::char_type c );

      void clear();
      void restore();

    private:
      glyph_sheet_iterator draw_glyph( charset::char_type c );
      glyph_sheet_iterator get_glyph_or_draw( charset::char_type c );

    private:
      /** \brief The interface to the true type library. */
      freetype_face m_font_face;

      /** \brief The glyph sheets on which the glyphs are drawn. */
      glyph_sheet_collection m_sheet;

      /** \brief This map provides for each draw character, the glyph sheet on
          which it was drawn. */
      character_to_glyph_sheet m_sheet_from_character;

    }; // class true_type_font
  } // namespace visual
} // namespace bear

#endif // __VISUAL_TRUE_TYPE_FONT_HPP__
