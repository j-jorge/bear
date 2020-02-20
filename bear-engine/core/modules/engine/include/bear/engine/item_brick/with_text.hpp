/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Inherit from this class to add some text in your item.
 * \author Julien Jorge
 */
#ifndef __ENGINE_WITH_TEXT_HPP__
#define __ENGINE_WITH_TEXT_HPP__

#include "bear/universe/types.hpp"
#include "bear/visual/font/font.hpp"
#include "bear/visual/writing.hpp"

#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief Inherit from this class to add some text in your item.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT with_text
    {
    public:
      with_text();

      void set_text( const std::string& text );
      const std::string& get_text() const;

      void set_font( const visual::font& spr );
      const visual::font& get_font() const;

      void set_horizontal_align( visual::text_align::horizontal_align a );
      visual::text_align::horizontal_align get_horizontal_align() const;

      void set_vertical_align( visual::text_align::vertical_align a );
      visual::text_align::vertical_align get_vertical_align() const;

      const visual::writing& get_writing() const;

      void fit_in_box( const universe::size_box_type& s );

      void refresh_writing();

    private:
      /** \brief The text. */
      std::string m_text;

      /** \brief The font used to display the text. */
      visual::font m_font;

      /** \brief How the text is aligned horizontally in the writing. */
      visual::text_align::horizontal_align m_horizontal_align;

      /** \brief How the text is aligned vertically in the writing. */
      visual::text_align::vertical_align m_vertical_align;

      /** \brief The writing displayed on the screen. */
      visual::writing m_writing;

    }; // class with_text
  } // namespace engine
} // namespace bear

#endif // __ENGINE_WITH_TEXT_HPP__
