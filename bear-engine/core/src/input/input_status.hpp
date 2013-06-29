/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class observing the state of keys/buttons.
 * \author Julien Jorge
 */
#ifndef __INPUT_INPUT_STATUS_HPP__
#define __INPUT_INPUT_STATUS_HPP__

#include "input/joystick_status.hpp"
#include "input/keyboard_status.hpp"
#include "input/mouse_status.hpp"

#include "input/class_export.hpp"

namespace bear
{
  namespace input
  {
    /**
     * \brief A class observing the state of keys/buttons.
     * \author Julien Jorge
     */
    class INPUT_EXPORT input_status
    {
    public:
      void read();

      void scan_inputs( input_listener& listener ) const;

    private:
      /** \brief Status of the keyboard's keys. */
      keyboard_status m_keyboard;

      /** \brief Status of the joysticks' buttons. */
      joystick_status m_joystick;

      /** \brief Status of the mouse's keys. */
      mouse_status m_mouse;

    }; // input_status
  } // namespace input
} // namespace bear

#endif // __INPUT_INPUT_STATUS_HPP__
