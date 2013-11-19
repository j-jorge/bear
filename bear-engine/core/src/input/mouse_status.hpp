/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class observing the state of the mouses' buttons.
 * \author Julien Jorge
 */
#ifndef __INPUT_MOUSE_STATUS_HPP__
#define __INPUT_MOUSE_STATUS_HPP__

#include "input/mouse.hpp"

#include "input/class_export.hpp"
#include <claw/ordered_set.hpp>

namespace bear
{
  namespace input
  {
    class input_listener;

    /**
     * \brief A class observing the state of the mouse's buttons.
     * \author Julien Jorge
     */
    class INPUT_EXPORT mouse_status
    {
    private:
      /** \brief The type of the set storing the keys. */
      typedef claw::math::ordered_set<mouse::mouse_code> set_type;

    public:
      mouse_status();

      void read();

      void scan_inputs( input_listener& listener ) const;

    private:
      /** \brief The set of the keys that have just been pressed. */
      set_type m_pressed;

      /** \brief The set of the keys that have just been released. */
      set_type m_released;

      /** \brief The set of the keys that are actually pressed. */
      set_type m_maintained;

      /** \brief The position of the cursor. */
      claw::math::coordinate_2d<unsigned int> m_cursor_pos;

      /** \brief Previous position of the cursor. */
      claw::math::coordinate_2d<unsigned int> m_previous_cursor_pos;

      /** \brief Tells if we did store a value in m_cursor_pos. */
      bool m_cursor_position_is_set;

    }; // mouse_status
  } // namespace input
} // namespace bear

#endif // __INPUT_MOUSE_STATUS_HPP__
