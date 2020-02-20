/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A radio_button is a checkable that cannot be unchecked by the user.
 * \author Julien Jorge
 */
#ifndef __GUI_RADIO_BUTTON_HPP__
#define __GUI_RADIO_BUTTON_HPP__

#include "bear/gui/checkable.hpp"

#include "bear/gui/class_export.hpp"

#include "bear/visual/font/font.hpp"
#include "bear/visual/sprite.hpp"

namespace bear
{
  namespace gui
  {
    /**
     * \brief A radio_button with a text.
     * \author Julien Jorge
     */
    class GUI_EXPORT radio_button:
      public checkable
    {
    public:
      radio_button( const visual::sprite& off, const visual::sprite& on );
      radio_button
        ( const visual::sprite& off, const visual::sprite& on, font_type f );

    private:
      bool on_key_press( const input::key_info& key );
      bool on_button_press
        ( input::joystick::joy_code button, unsigned int joy_index );
      bool on_mouse_press
        ( input::mouse::mouse_code key,
          const claw::math::coordinate_2d<unsigned int>& pos );

    }; // class radio_button
  } // namespace gui
} // namespace bear

#endif // __GUI_RADIO_BUTTON_HPP__
