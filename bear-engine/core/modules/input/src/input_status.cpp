/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::input::input_status class.
 * \author Julien Jorge
 */
#include "bear/input/input_status.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the status of the keys.
 */
void bear::input::input_status::read()
{
  m_keyboard.read();
  m_joystick.read();
  m_mouse.read();
  m_finger.read();
} // input_status::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send the status of the keys to an input_listener.
 * \param listener The listener to pass informations to.
 */
void bear::input::input_status::scan_inputs( input_listener& listener ) const
{
  m_keyboard.scan_inputs( listener );
  m_joystick.scan_inputs( listener );
  m_mouse.scan_inputs( listener );
  m_finger.scan_inputs( listener );
} // input_status::scan_inputs()
