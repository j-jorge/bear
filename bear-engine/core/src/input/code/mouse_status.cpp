/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::input::mouse_status class.
 * \author Julien Jorge
 */
#include "input/mouse_status.hpp"

#include "input/input_listener.hpp"
#include "input/system.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::input::mouse_status::mouse_status()
  : m_cursor_position_is_set(false)
{

} // mouse_status::mouse_status()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the status of the keys.
 */
void bear::input::mouse_status::read()
{
  const mouse& ms = system::get_instance().get_mouse();
  mouse::const_iterator it;
  set_type current;

  for (it=ms.begin(); it!=ms.end(); ++it)
    current.insert(*it);

  m_released = m_pressed;
  m_released.join(m_maintained);
  m_released.difference(current);
  m_maintained.join(m_pressed).intersection(current);
  (m_pressed = current).difference(m_maintained);

  m_forget_button.difference( m_released );

  if ( m_cursor_position_is_set )
    {
      m_previous_cursor_pos = m_cursor_pos;
      m_cursor_pos = ms.get_position();
    }
  else
    {
      m_cursor_pos = ms.get_position();
      m_previous_cursor_pos = m_cursor_pos;
      m_cursor_position_is_set = true;
    }
} // mouse_status::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send the status of the keys to an input_listener.
 * \param listener The listener to pass informations to.
 */
void bear::input::mouse_status::scan_inputs( input_listener& listener ) const
{
  set_type::const_iterator it;

  for (it=m_pressed.begin(); it!=m_pressed.end(); ++it)
    listener.mouse_pressed( *it, m_cursor_pos );

  for (it=m_maintained.begin(); it!=m_maintained.end(); ++it)
    listener.mouse_maintained( *it, m_cursor_pos );

  for (it=m_released.begin(); it!=m_released.end(); ++it)
    listener.mouse_released( *it, m_cursor_pos );

  if ( m_cursor_pos != m_previous_cursor_pos )
    listener.mouse_move( m_cursor_pos );
} // mouse_status::scan_inputs()
