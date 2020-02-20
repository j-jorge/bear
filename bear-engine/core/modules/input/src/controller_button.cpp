/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Informations on a pressed key.
 * \author Julien Jorge
 */
#include "bear/input/controller_button.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::input::controller_button::controller_button()
  : m_type(controller_not_set)
{

} // controller_button::controller_button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param key The keyboard key to store.
 */
bear::input::controller_button::controller_button( const key_info& key )
  : m_type(controller_keyboard), m_keyboard(key)
{

} // controller_button::controller_button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param joy The joystick button to store.
 */
bear::input::controller_button::controller_button( const joystick_button& joy )
  : m_type(controller_joystick), m_joystick(joy)
{

} // controller_button::controller_button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param m The mouse button to store.
 */
bear::input::controller_button::controller_button( mouse::mouse_code m )
  : m_type(controller_mouse), m_mouse(m)
{

} // controller_button::controller_button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 * \param key The keyboard key to store.
 */
bear::input::controller_button&
bear::input::controller_button::operator=( const key_info& key )
{
  m_type = controller_keyboard;
  m_keyboard = key;
  return *this;
} // controller_button::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignement operator.
 * \param joy The joystick button to store.
 */
bear::input::controller_button&
bear::input::controller_button::operator=( const joystick_button& joy )
{
  m_type = controller_joystick;
  m_joystick = joy;
  return *this;
} // controller_button::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 * \param m The mouse button to store.
 */
bear::input::controller_button&
bear::input::controller_button::operator=( mouse::mouse_code m )
{
  m_type = controller_mouse;
  m_mouse = m;
  return *this;
} // controller_button::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the type of the controller for which the button is stored.
 */
bear::input::controller_button::controller_type
bear::input::controller_button::get_type() const
{
  return m_type;
} // controller_button::get_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the keyboard button.
 */
const bear::input::key_info&
bear::input::controller_button::get_key_info() const
{
  CLAW_PRECOND( m_type == controller_keyboard );
  return m_keyboard;
} // controller_button::get_key_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the joystick button.
 */
const bear::input::joystick_button&
bear::input::controller_button::get_joystick_button() const
{
  CLAW_PRECOND( m_type == controller_joystick );
  return m_joystick;
} // controller_button::get_joystick_button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the mouse button.
 */
bear::input::mouse::mouse_code
bear::input::controller_button::get_mouse_code() const
{
  CLAW_PRECOND( m_type == controller_mouse );
  return m_mouse;
} // controller_button::get_mouse_code()
