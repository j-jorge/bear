/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class representing a joystick.
 * \author Julien Jorge
 */
#ifndef __INPUT_JOYSTICK_HPP__
#define __INPUT_JOYSTICK_HPP__

#include <string>
#include <vector>
#include <list>

#include <SDL/SDL.h>

#include <claw/non_copyable.hpp>

#include "input/class_export.hpp"

namespace bear
{
  namespace input
  {
    /**
     * \brief A class representing a joystick.
     */
    class INPUT_EXPORT joystick:
      public claw::pattern::non_copyable
    {
    public:
      /** \brief Code representing a button on the joystick. */
      typedef unsigned int joy_code;

      /** \brief Iterator on the pressed buttons. */
      typedef std::list<joy_code>::const_iterator const_iterator;

    public:
      static unsigned int number_of_joysticks();

      joystick(unsigned int joy_id);
      ~joystick();

      static std::string get_name_of( joy_code k );
      static std::string get_translated_name_of( joy_code k );
      static joy_code get_code_named( const std::string& n );

      const_iterator begin() const;
      const_iterator end() const;
      bool empty() const;

      unsigned int get_id() const;

      // only for input::system
      void refresh();

    private:
      joy_code get_pressed_axis() const;
      joy_code sdl_button_to_local( unsigned int sdl_val ) const;

    public:
#include "input/joy_codes.hpp"

    private:
      /** \brief Pressed buttons. */
      std::list<joy_code> m_pressed_buttons;

      /** \brief Identifier of the opened joystick. */
      const unsigned int m_id;

      /** \brief SDL joystick. */
      SDL_Joystick* m_joystick;

    }; // class joystick
  } // namespace input
} // namespace bear

#endif // __INPUT_JOYSTICK_HPP__
