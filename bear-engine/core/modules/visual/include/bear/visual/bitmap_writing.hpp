/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A text written with some bitmap font.
 * \author Julien Jorge
 */
#ifndef __VISUAL_BITMAP_WRITING_HPP__
#define __VISUAL_BITMAP_WRITING_HPP__

#include "bear/visual/class_export.hpp"

#include "bear/visual/placed_sprite.hpp"
#include "bear/visual/sequence_effect.hpp"
#include "bear/visual/text_align.hpp"

#include <vector>
#include <string>

namespace bear
{
  namespace visual
  {
    class font;
    class base_screen;
    class scene_writing;

    /**
     * \brief A text written with some bitmap font.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT bitmap_writing:
      public bitmap_rendering_attributes
    {
    private:
      typedef std::vector<placed_sprite> sprite_list;

    private:
      /**
       * \brief The class passed to text_layout to compute the position of the
       *        sprites of the text.
       */
      class arrange_sprite_list
      {
      public:
        arrange_sprite_list
        ( const font& f, const std::string& str, sprite_list& list );

        void operator()
          ( position_type p, std::size_t first, std::size_t last );

        coordinate_type get_bottom() const;

      private:
        // Not implemented
        arrange_sprite_list( const arrange_sprite_list& );

        // Not implemented
        arrange_sprite_list& operator=( const arrange_sprite_list& );

      private:
        /** \brief The text to arrange. */
        std::string const& m_text;

        /** \brief The font used to display the text. */
        font const& m_font;

        /** \brief The sprites of the text. */
        sprite_list& m_sprites;

        /** \brief The bottom of the text on the screen. */
        coordinate_type m_bottom;

      }; // class arrange_sprite_list

    public:
      typedef sprite_list::const_iterator const_iterator;

    public:
      std::size_t get_sprites_count() const;
      placed_sprite get_sprite( std::size_t i ) const;

      void set_effect( sequence_effect e );
      void update( double t );

      void create
        ( const font& f, const std::string& str,
          const size_box_type& s, text_align::horizontal_align h,
          text_align::vertical_align v );

      void call_render( const scene_writing& s, base_screen& scr ) const;

    private:
      void shift_vertically( coordinate_type offset );

    private:
      /** \brief The sprites that make the text. */
      sprite_list m_sprites;

      /** \brief The effect to apply to the sprites. */
      sequence_effect m_effect;

    }; // class bitmap_writing
  } // namespace visual
} // namespace bear

#endif // __VISUAL_BITMAP_WRITING_HPP__
