/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::input::input_status class.
 * \author Julien Jorge
 */
#include "input/input_status.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the status of the keys.
 */
void bear::input::input_status::read()
{
  m_keyboard.read();
  m_joystick.read();
  m_mouse.read();
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
} // input_status::scan_inputs()
