/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Base class for classes who want to be informed about the state of the
 *        keyboard's keys or joysticks' buttons.
 * \author Julien Jorge
 */
#ifndef __INPUT_INPUT_LISTENER_HPP__
#define __INPUT_INPUT_LISTENER_HPP__

#include "input/joystick.hpp"
#include "input/mouse.hpp"

#include "input/class_export.hpp"

namespace bear
{
  namespace input
  {
    class key_info;

    /**
     * \brief Base class for classes who want to be informed about the state of
     *        the keyboard's keys or joysticks' buttons.
     * \author Julien Jorge
     */
    class INPUT_EXPORT input_listener
    {
    public:
      virtual ~input_listener();

      virtual bool key_pressed( const key_info& key );
      virtual bool key_released( const key_info& key );
      virtual bool key_maintained( const key_info& key );
      virtual bool char_pressed( const key_info& key );

      virtual bool button_pressed
      ( joystick::joy_code button, unsigned int joy_index );
      virtual bool button_released
      ( joystick::joy_code button, unsigned int joy_index );
      virtual bool button_maintained
      ( joystick::joy_code button, unsigned int joy_index );

      virtual
      bool mouse_pressed
      ( mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );
      virtual
      bool mouse_released
      ( mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );
      virtual
      bool mouse_maintained
      ( mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );

      virtual
      bool mouse_move( const claw::math::coordinate_2d<unsigned int>& pos );

    }; // class input_listener
  } // namespace input
} // namespace bear

#endif // __INPUT_INPUT_LISTENER_HPP__
