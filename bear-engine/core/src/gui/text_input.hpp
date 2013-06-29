/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A component that display a modifiable text.
 * \author Julien Jorge
 */
#ifndef __GUI_TEXT_INPUT_HPP__
#define __GUI_TEXT_INPUT_HPP__

#include "gui/callback_group.hpp"
#include "gui/visual_component.hpp"
#include "input/keyboard.hpp"
#include "visual/font/font.hpp"

#include "gui/class_export.hpp"

namespace bear
{
  namespace gui
  {
    class static_text;

    /**
     * \brief A component that display a modifiable text.
     * \author Julien Jorge
     */
    class GUI_EXPORT text_input:
      public visual_component
    {
    public:
      /** \brief The type of the font used to display the text. */
      typedef visual::font font_type;

    public:
      text_input( font_type f, visual::color_type cursor_color );

      void clear();
      void set_text( const std::string& text );
      const std::string&  get_text() const;

      void add_enter_callback( const callback& c );
      void add_changed_callback( const callback& c );

      void set_margin( coordinate_type x, coordinate_type y );
      void set_margin( const size_box_type& m );

    private:
      bool on_key_press( const input::key_info& key );
      bool on_char_pressed( const input::key_info& key );

      void on_resized();

      void display( std::list<visual::scene_element>& e ) const;

      void insert_character( char key );
      bool special_code( const input::key_info& key );

      void move_left();
      void move_right();
      void adjust_text_by_left();
      void adjust_text_by_right();

      void adjust_visible_part_of_text();

    private:
      /** \brief A static text, used for displaying the visible part of the
          text. */
      static_text* m_static_text;

      /** \brief Position for next character. */
      unsigned int m_cursor;

      /** \brief The input text. */
      std::string m_text;

      /** \brief The color of the cursor. */
      visual::color_type m_cursor_color;

      /** \brief Position of the first visible character in the text. */
      std::string::size_type m_first;

      /** \brief Position of the last visible character in the text. */
      std::string::size_type m_last;

      /** \brief Maximum number of characters in the line. */
      std::string::size_type m_line_length;

      /** \brief Callback executed when enter is pressed. */
      callback_group m_enter_callback;

      /** \brief Callback executed when the text has changed. */
      callback_group m_changed_callback;

    }; // class text_input
  } // namespace gui
} // namespace bear

#endif // __GUI_TEXT_INPUT_HPP__
