/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A checkable is something that can be checked, next to a text.
 * \author Julien Jorge
 */
#ifndef __GUI_CHECKABLE_HPP__
#define __GUI_CHECKABLE_HPP__

#include "gui/callback_group.hpp"
#include "gui/visual_component.hpp"

#include "gui/class_export.hpp"

#include "visual/font/font.hpp"
#include "visual/sprite.hpp"

namespace bear
{
  namespace gui
  {
    class static_text;

    /**
     * \brief A checkable is something that can be checked, next to a text.
     * \author Julien Jorge
     */
    class GUI_EXPORT checkable:
      public visual_component
    {
    public:
      /** \brief The type of the font used to display the text. */
      typedef visual::font font_type;

    public:
      checkable( const visual::sprite& off, const visual::sprite& on );
      checkable
        ( const visual::sprite& off, const visual::sprite& on, font_type f );

      void set_font( font_type f );
      void set_text( const std::string& text );
      void check( bool b = true );
      void toggle_value();
      void set_value( bool b );

      const std::string& get_text() const;
      bool checked() const;

      void add_checked_callback( const callback& c );
      void add_unchecked_callback( const callback& c );

    private:
      void display( scene_element_list& e ) const;

      void create();
      void fit();

      void adjust_text_position();

    private:
      /** \brief The text to draw. */
      static_text* m_text;

      /** \brief Tell if the box is checked or not. */
      bool m_checked;

      /** \brief The sprite displayed when the box is not checked. */
      const visual::sprite m_off;

      /** \brief The sprite displayed when the box is checked. */
      const visual::sprite m_on;

      /** \brief Callback executed when the checkable is checked. */
      callback_group m_checked_callback;

      /** \brief Callback executed when the checkable is unchecked. */
      callback_group m_unchecked_callback;

    }; // class checkable
  } // namespace gui
} // namespace bear

#endif // __GUI_CHECKABLE_HPP__
