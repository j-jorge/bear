/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class observing the state of the keyboard's buttons.
 * \author Julien Jorge
 */
#ifndef __INPUT_KEYBOARD_STATUS_HPP__
#define __INPUT_KEYBOARD_STATUS_HPP__

#include "input/keyboard.hpp"

#include <claw/ordered_set.hpp>

#include "input/class_export.hpp"

namespace bear
{
  namespace input
  {
    class input_listener;

    /**
     * \brief A class observing the state of the keyboard's buttons.
     * \author Julien Jorge
     */
    class INPUT_EXPORT keyboard_status
    {
    private:
      /** \brief The type of the set storing the keys. */
      typedef claw::math::ordered_set<key_code> set_type;

      /** \brief The type of the event list of the keyboard. */
      typedef keyboard::event_list event_list;

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

      /** \brief Keys that must not be taken into account. */
      set_type m_forget_key;

      /** \brief The key events. */
      event_list m_events;

    }; // keyboard_status
  } // namespace input
} // namespace bear

#endif // __INPUT_KEYBOARD_STATUS_HPP__
