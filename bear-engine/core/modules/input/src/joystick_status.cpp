/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::input::joystick_status class.
 * \author Julien Jorge
 */
#include "bear/input/joystick_status.hpp"

#include "bear/input/input_listener.hpp"
#include "bear/input/system.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the status of the keys.
 */
void bear::input::joystick_status::read()
{
  const unsigned int nb_joy = joystick::number_of_joysticks();
  set_type current;

  for (unsigned int i=0; i!=nb_joy; ++i)
    {
      const joystick& joy = system::get_instance().get_joystick(i);
      joystick::const_iterator it;

      for (it=joy.begin(); it!=joy.end(); ++it)
        current.insert( joystick_button(i, *it) );
    }

  (m_released = m_pressed).join(m_maintained).difference(current);
  m_maintained.join(m_pressed).intersection(current);
  (m_pressed = current).difference(m_maintained);

  m_forget_button.difference( m_released );
} // joystick_status::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send the status of the keys to an input_listener.
 * \param listener The listener to pass informations to.
 */
void bear::input::joystick_status::scan_inputs( input_listener& listener ) const
{
  set_type::const_iterator it;

  for (it=m_pressed.begin(); it!=m_pressed.end(); ++it)
    switch (it->button)
      {
      case joystick::jc_axis_up_left:
        listener.button_pressed( joystick::jc_axis_up, it->joystick_index );
        listener.button_pressed( joystick::jc_axis_left, it->joystick_index );
        break;
      case joystick::jc_axis_up_right:
        listener.button_pressed( joystick::jc_axis_up, it->joystick_index );
        listener.button_pressed( joystick::jc_axis_right, it->joystick_index );
        break;
      case joystick::jc_axis_down_left:
        listener.button_pressed( joystick::jc_axis_down, it->joystick_index );
        listener.button_pressed( joystick::jc_axis_left, it->joystick_index );
        break;
      case joystick::jc_axis_down_right:
        listener.button_pressed( joystick::jc_axis_down, it->joystick_index );
        listener.button_pressed( joystick::jc_axis_right, it->joystick_index );
        break;
      default:
        listener.button_pressed( it->button, it->joystick_index );
      }

  for (it=m_maintained.begin(); it!=m_maintained.end();
       ++it)
    switch (it->button)
      {
      case joystick::jc_axis_up_left:
        listener.button_maintained( joystick::jc_axis_up, it->joystick_index );
        listener.button_maintained
          ( joystick::jc_axis_left, it->joystick_index );
        break;
      case joystick::jc_axis_up_right:
        listener.button_maintained( joystick::jc_axis_up, it->joystick_index );
        listener.button_maintained
          ( joystick::jc_axis_right, it->joystick_index );
        break;
      case joystick::jc_axis_down_left:
        listener.button_maintained
          ( joystick::jc_axis_down, it->joystick_index );
        listener.button_maintained
          ( joystick::jc_axis_left, it->joystick_index );
        break;
      case joystick::jc_axis_down_right:
        listener.button_maintained
          ( joystick::jc_axis_down, it->joystick_index );
        listener.button_maintained
          ( joystick::jc_axis_right, it->joystick_index );
        break;
      default:
        listener.button_maintained( it->button, it->joystick_index );
      }

  for (it=m_released.begin(); it!=m_released.end(); ++it)
    switch (it->button)
      {
      case joystick::jc_axis_up_left:
        listener.button_released( joystick::jc_axis_up, it->joystick_index );
        listener.button_released( joystick::jc_axis_left, it->joystick_index );
        break;
      case joystick::jc_axis_up_right:
        listener.button_released( joystick::jc_axis_up, it->joystick_index );
        listener.button_released( joystick::jc_axis_right, it->joystick_index );
        break;
      case joystick::jc_axis_down_left:
        listener.button_released( joystick::jc_axis_down, it->joystick_index );
        listener.button_released( joystick::jc_axis_left, it->joystick_index );
        break;
      case joystick::jc_axis_down_right:
        listener.button_released( joystick::jc_axis_down, it->joystick_index );
        listener.button_released( joystick::jc_axis_right, it->joystick_index );
        break;
      default:
        listener.button_released( it->button, it->joystick_index );
      }
} // joystick_status::scan_inputs()
