/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Simple component to print a text on the screen.
 * \author Julien Jorge
 */
#ifndef __GUI_STATIC_TEXT_HPP__
#define __GUI_STATIC_TEXT_HPP__

#include "gui/visual_component.hpp"
#include "visual/font/font.hpp"
#include "visual/writing.hpp"

#include <claw/coordinate_2d.hpp>

#include "gui/class_export.hpp"

namespace bear
{
  namespace gui
  {
    /**
     * \brief Simple component to print a text on the screen.
     * \author Julien Jorge
     */
    class GUI_EXPORT static_text:
      public visual_component
    {
    public:
      /** \brief The type of the font used to display the text. */
      typedef visual::font font_type;

    private:
      /** \brief The class passed to text_layout to compute the longest text
          that can be displayed. */
      class arrange_longest_text
      {
      public:
        arrange_longest_text( std::size_t& result );

        void operator()
          ( visual::position_type p, std::size_t first, std::size_t last );

      private:
        /** \brief The length of the text. */
        std::size_t& m_result;

      }; // class arrange_sprite_list

    public:
      static_text();
      explicit static_text( font_type f );

      void set_font( font_type f );
      void set_auto_size( bool b );
      void clear();

      void set_text( const std::string& text );

      const std::string& get_text() const;
      font_type get_font() const;

      void expand_vertically();

      std::size_t
      get_longest_text( const std::string& text, std::size_t i ) const;

      void set_margin( coordinate_type x, coordinate_type y );
      void set_margin( const size_box_type& m );

      size_type get_min_height_with_text() const;

      visual::bitmap_rendering_attributes& get_text_rendering_attributes();
      const visual::bitmap_rendering_attributes&
        get_text_rendering_attributes() const;

    private:
      size_box_type get_auto_size_with_max_width( size_type w ) const;

      void display( std::list<visual::scene_element>& e ) const;

      void adjust_size_to_text();

      void on_resized();

      void refresh_writing();

    private:
      /** \brief The text to draw. */
      std::string m_text;

      /** \brief The font used to draw the text. */
      font_type m_font;

      /** \brief Tell if we must adjust the size of the component to the size of
          the text. */
      bool m_auto_size;

      /** \brief The text displayed. */
      visual::writing m_writing;

      /** \brief The margins around the text. */
      visual::size_box_type m_margin;

      /** \brief Rendering attributes of the text. */
      visual::bitmap_rendering_attributes m_text_rendering_attributes;
    }; // class static_text
  } // namespace gui
} // namespace bear

#endif // __GUI_STATIC_TEXT_HPP__
