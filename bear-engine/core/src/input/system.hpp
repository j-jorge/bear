/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class containing initialization and releasing methods for using the
 *        classes of the namespace input.
 * \author Julien Jorge
 */
#ifndef __INPUT_SYSTEM_HPP__
#define __INPUT_SYSTEM_HPP__

#include <claw/basic_singleton.hpp>
#include <vector>

#include "input/class_export.hpp"

namespace bear
{
  namespace input
  {
    class joystick;
    class keyboard;
    class mouse;

    /**
     * \brief A class containing initialization and releasing methods for using
     *        the classes of the namespace input.
     */
    class INPUT_EXPORT system:
      public claw::pattern::basic_singleton<system>
    {
      // need an access to the constructor/destructor.
      friend class claw::pattern::basic_singleton<system>;

      typedef claw::pattern::basic_singleton<system> super;

    public:
      static void initialize();
      static void release();

      // Must be redefined to work correctly with dynamic libraries.
      // At least under Windows with MinGW.
      static system& get_instance();

      void refresh();

      keyboard& get_keyboard();
      mouse& get_mouse();
      joystick& get_joystick( unsigned int joy_id );

    private:
      system();
      ~system();

      void refresh_alone();

      void clear();

    private:
      /** \brief The keyboard. */
      keyboard* m_keyboard;

      /** \brief The mouse. */
      mouse* m_mouse;

      /** \brief The joysticks. */
      std::vector<joystick*> m_joystick;

    }; // class system
  } // namespace input
} // namespace bear

#endif // __INPUT_SYSTEM_HPP__
