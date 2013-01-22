/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::input::keyboard_status class.
 * \author Julien Jorge
 */
#include "input/keyboard_status.hpp"

#include "input/input_listener.hpp"
#include "input/system.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the status of the keys.
 */
void bear::input::keyboard_status::read()
{
  const keyboard& kb = system::get_instance().get_keyboard();
  keyboard::const_iterator it;
  set_type current;

  for (it=kb.begin(); it!=kb.end(); ++it)
    current.insert(*it);

  m_released = m_pressed;
  m_released.join(m_maintained);
  m_released.difference(current);
  m_maintained.join(m_pressed).intersection(current);
  (m_pressed = current).difference(m_maintained);

  m_forget_key.difference( m_released );

  m_events = kb.get_events();
} // keyboard_status::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send the status of the keys to an input_listener.
 * \param listener The listener to pass informations to.
 */
void bear::input::keyboard_status::scan_inputs( input_listener& listener ) const
{
  set_type::const_iterator it;

  for (it=m_pressed.begin(); it!=m_pressed.end(); ++it)
    listener.key_pressed( key_info(*it) );

  for (it=m_maintained.begin(); it!=m_maintained.end(); ++it)
    listener.key_maintained( key_info(*it) );

  for (it=m_released.begin(); it!=m_released.end(); ++it)
    listener.key_released( key_info(*it) );

  event_list::const_iterator itk;
  for ( itk=m_events.begin(); itk!=m_events.end(); ++itk )
    switch( itk->get_type() )
      {
      case key_event::key_event_character:
        listener.char_pressed( itk->get_info() );
        break;
      default:
        { /* ignored */ }
      }
} // keyboard_status::scan_inputs()
