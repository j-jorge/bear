/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A button on which the user click to produce an action.
 * \author Julien Jorge
 */
#ifndef __GUI_BUTTON_HPP__
#define __GUI_BUTTON_HPP__

#include "gui/callback_group.hpp"
#include "gui/visual_component.hpp"

#include "gui/class_export.hpp"

#include "visual/font/font.hpp"

namespace bear
{
  namespace gui
  {
    class picture;
    class static_text;

    /**
     * \brief A button on which the user click to produce an action.
     * \author Julien Jorge
     */
    class GUI_EXPORT button:
      public visual_component
    {
    public:
      /** \brief The type of the font used to display the text. */
      typedef visual::font font_type;

    public:
      button( const font_type& f, const std::string& label );
      button( const font_type& f, const std::string& label, const callback& c );
      button( const visual::sprite& icon );

      void set_font( font_type f );
      void set_margin( size_type m );
      void set_text( const std::string& text );
      void set_icon( const visual::sprite& icon );

      const std::string& get_text() const;

      void add_callback( const callback& c );

    private:
      void create();

      void adjust_component_positions();

      bool on_key_press( const input::key_info& key );
      bool on_button_press
        ( input::joystick::joy_code joy_button, unsigned int joy_index );
      bool on_mouse_press
        ( input::mouse::mouse_code key,
          const claw::math::coordinate_2d<unsigned int>& pos );
      bool on_finger_action( const input::finger_event& event );

    private:
      /** \brief The text in the button. */
      static_text* m_text;

      /** \brief The icon in the button. */
      picture* m_icon;

      /** \brief Callback executed when the button is clicked. */
      callback_group m_click_callback;

      /** \brief The size of the margin around the text of the button. */
      size_type m_margin;

    }; // class button
  } // namespace gui
} // namespace bear

#endif // __GUI_BUTTON_HPP__
