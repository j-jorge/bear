/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A checkbox with a text.
 * \author Julien Jorge
 */
#ifndef __GUI_CHECKBOX_HPP__
#define __GUI_CHECKBOX_HPP__

#include "gui/checkable.hpp"

#include "gui/class_export.hpp"

#include "visual/font/font.hpp"
#include "visual/sprite.hpp"

namespace bear
{
  namespace gui
  {
    /**
     * \brief A checkbox with a text.
     * \author Julien Jorge
     */
    class GUI_EXPORT checkbox:
      public checkable
    {
    public:
      checkbox( const visual::sprite& off, const visual::sprite& on );
      checkbox
        ( const visual::sprite& off, const visual::sprite& on, font_type f );

    private:
      bool on_key_press( const input::key_info& key );
      bool on_button_press
        ( input::joystick::joy_code button, unsigned int joy_index );
      bool on_mouse_press
        ( input::mouse::mouse_code key,
          const claw::math::coordinate_2d<unsigned int>& pos );

    }; // class checkbox
  } // namespace gui
} // namespace bear

#endif // __GUI_CHECKBOX_HPP__
