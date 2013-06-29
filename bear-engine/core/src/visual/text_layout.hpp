/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class that arrange a text in a box (like it should be displayed).
 * \author Julien Jorge
 */
#ifndef __VISUAL_TEXT_LAYOUT_HPP__
#define __VISUAL_TEXT_LAYOUT_HPP__

#include "visual/font/font.hpp"
#include "visual/text_align.hpp"

#include "visual/class_export.hpp"

#include <string>

namespace bear
{
  namespace visual
  {
    /**
     * \brief A class that arrange a text in a box (like it should be
     *        displayed).
     *
     * \sa writing
     *
     * \author Julien Jorge
     */
    class text_layout
    {
    private:
      /** \brief The type of the cursor representing the position of the next
          character. */
      typedef position_type cursor_type;

    public:
      VISUAL_EXPORT text_layout
      ( const font& f, const std::string& str, const size_box_type& s,
        text_align::horizontal_align a = text_align::align_left );

      template<typename Func>
      void arrange_text( Func func ) const;

    private:
      template<typename Func>
      void arrange_next_word
      ( Func func, cursor_type& cursor, std::size_t& i ) const;

      template<typename Func>
      void arrange_word
      ( Func func, cursor_type& cursor, std::size_t& i,
        const std::size_t n ) const;

      VISUAL_EXPORT coordinate_type compute_line_left
      ( std::size_t first ) const;
      VISUAL_EXPORT size_type compute_line_width( std::size_t first ) const;
      VISUAL_EXPORT size_type
      compute_line_height_above_baseline( std::size_t first ) const;

    private:
      /** \brief The size of the box around the text. */
      const size_box_type& m_size;

      /** \brief The text to arrange. */
      const std::string& m_text;

      /** \brief The font used to display the text. */
      const font& m_font;

      /** \brief How to align the lines of text. */
      text_align::horizontal_align m_horizontal_align;

    }; // class text_layout
  } // namespace visual
} // namespace bear

#include "visual/impl/text_layout.tpp"

#endif // __VISUAL_TEXT_LAYOUT_HPP__
