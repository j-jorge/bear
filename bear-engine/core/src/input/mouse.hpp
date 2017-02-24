/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class representing a mouse.
 * \author Julien Jorge
 */
#ifndef __INPUT_MOUSE_HPP__
#define __INPUT_MOUSE_HPP__

#include "input/class_export.hpp"
#include "input/display_projection.hpp"

#include <string>
#include <vector>
#include <unordered_set>

#include <SDL2/SDL.h>

#include <claw/math.hpp>

namespace bear
{
  namespace input
  {
    /**
     * \brief A class representing a mouse.
     */
    class INPUT_EXPORT mouse
    {
    public:
      /** \brief Code representing a button on the mouse. */
      typedef unsigned char mouse_code;

      /** \brief Iterator on the pressed buttons. */
      typedef std::unordered_set<mouse_code>::const_iterator const_iterator;

    public:
      mouse();

      static std::string get_name_of( mouse_code b );
      static std::string get_translated_name_of( mouse_code b );
      static mouse_code get_button_named( const std::string& n );

      const_iterator begin() const;
      const_iterator end() const;
      bool empty() const;

      void set_display( const display_projection& display );
      const claw::math::coordinate_2d<unsigned int>& get_position() const;

      // only for input::system
      void refresh();

    public:
#include "input/mouse_codes.hpp"
      
    private:
      void process_button_down_event( const SDL_MouseButtonEvent* evt );
      void process_button_up_event( const SDL_MouseButtonEvent* evt );
      void process_wheel_event( const SDL_MouseWheelEvent* evt );

      void update_position();
      mouse_code sdl_button_to_local( unsigned int sdl_val ) const;

    private:
      display_projection m_display;
      
      /** \brief The last result of refresh(). */
      std::unordered_set<mouse_code> m_current_state;

      /** \brief Pressed buttons, does not include the wheel. */
      std::unordered_set<mouse_code> m_pressed_buttons;

      /** \brief Position of the cursor. */
      claw::math::coordinate_2d<unsigned int> m_position;

    }; // class mouse
  } // namespace input
} // namespace bear

#endif // __INPUT_MOUSE_HPP__
