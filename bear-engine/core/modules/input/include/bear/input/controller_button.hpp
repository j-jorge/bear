/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Informations on a pressed key.
 * \author Julien Jorge
 */
#ifndef __INPUT_CONTROLLER_BUTTON_HPP__
#define __INPUT_CONTROLLER_BUTTON_HPP__

#include "bear/input/key_info.hpp"
#include "bear/input/joystick_button.hpp"
#include "bear/input/mouse.hpp"

#include "bear/input/class_export.hpp"

namespace bear
{
  namespace input
  {
    /**
     * \brief Informations on a pressed key.
     * \author Julien Jorge
     */
    class INPUT_EXPORT controller_button
    {
    public:
      /** \brief The type of the controller for which the button is stored. */
      enum controller_type
        {
          controller_keyboard,
          controller_joystick,
          controller_mouse,
          controller_not_set
        }; // enum controller_type

    public:
      controller_button();
      controller_button( const key_info& key );
      controller_button( const joystick_button& joy );
      controller_button( mouse::mouse_code m );

      controller_button& operator=( const key_info& key );
      controller_button& operator=( const joystick_button& joy );
      controller_button& operator=( mouse::mouse_code m );

      controller_type get_type() const;
      const key_info& get_key_info() const;
      const joystick_button& get_joystick_button() const;
      mouse::mouse_code get_mouse_code() const;

    private:
      /** \brief The type of the controller for which the button is stored. */
      controller_type m_type;

      /** \brief The keyboard key, when the button is a keyboard one. */
      key_info m_keyboard;

      /** \brief The joystick button, when the button is a joystick one. */
      joystick_button m_joystick;

      /** \brief The mouse button, when the button is a mouse one. */
      mouse::mouse_code m_mouse;

    }; // class controller_button

  } // namespace input
} // namespace bear

#endif // __INPUT_CONTROLLER_BUTTON_HPP__
