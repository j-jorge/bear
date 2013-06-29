/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class observing the state of the joysticks' buttons.
 * \author Julien Jorge
 */
#ifndef __INPUT_JOYSTICK_STATUS_HPP__
#define __INPUT_JOYSTICK_STATUS_HPP__

#include "input/joystick_button.hpp"

#include <claw/ordered_set.hpp>

#include "input/class_export.hpp"

namespace bear
{
  namespace input
  {
    class input_listener;

    /**
     * \brief A class observing the state of the joysticks' buttons.
     * \author Julien Jorge
     */
    class INPUT_EXPORT joystick_status
    {
    private:
      /** \brief The type of the set storing the keys. */
      typedef claw::math::ordered_set<joystick_button> set_type;

    public:
      void read();

      void scan_inputs( input_listener& listener ) const;

    private:
      /** \brief The set of the keys that have just been pressed. */
      set_type m_pressed;

      /** \brief The set of the keys that have just been released. */
      set_type m_released;

      /** \brief The set of the keys that are actually pressed. */
      set_type m_maintained;

      /** \brief Buttons that must not be taken into account. */
      set_type m_forget_button;

    }; // joystick_status
  } // namespace input
} // namespace bear

#endif // __INPUT_JOYSTICK_STATUS_HPP__
