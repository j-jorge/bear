/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class stores a button of a joystick and the index of this
 *        joystick.
 * \author Julien Jorge
 */
#ifndef __INPUT_JOYSTICK_BUTTON_HPP__
#define __INPUT_JOYSTICK_BUTTON_HPP__

#include "input/joystick.hpp"

#include "input/class_export.hpp"

namespace bear
{
  namespace input
  {
    /**
     * \brief This class stores a button of a joystick and the index of this
     *        joystick.
     * \author Julien Jorge
     */
    class INPUT_EXPORT joystick_button
    {
    public:
      joystick_button();
      joystick_button( unsigned int joy, input::joystick::joy_code b );

      bool operator<( const joystick_button& that ) const;
      bool operator==( const joystick_button& that ) const;

      static std::string get_name_of( const joystick_button& j );
      static std::string get_translated_name_of( const joystick_button& j );
      static joystick_button get_button_named( const std::string& n );

    public:
      /** \brief The index of the joystick containing the key. */
      unsigned int joystick_index;

      /** \brief The code of the button. */
      joystick::joy_code button;

    }; // class joystick_button
  } // namespace input
} // namespace bear

#endif // __INPUT_JOYSTICK_BUTTON_HPP__
