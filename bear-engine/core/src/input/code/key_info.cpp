/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::input::key_info class.
 * \author Julien Jorge
 */
#include "input/key_info.hpp"

#include "input/keyboard.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bear::input::key_info::key_info()
{

} // key_info::key_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor from a key_code.
 * \param c The code of the key.
 */
bear::input::key_info::key_info( key_code c )
  : m_code(c), m_symbol(0)
{

} // key_info::key_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor from a key_code and a character.
 * \param c The code of the key.
 * \param s The character.
 */
bear::input::key_info::key_info( key_code c, charset::char_type s )
  : m_code(c), m_symbol(s)
{

} // key_info::key_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two key_info are equals.
 * \param that The key_info to compare to.
 */
bool bear::input::key_info::operator==( const key_info& that ) const
{
  return (m_code == that.m_code) && (m_symbol == that.m_symbol);
} // key_info::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is lower than an other.
 * \param that The key_info to compare to.
 */
bool bear::input::key_info::operator<( const key_info& that ) const
{
  if ( m_code < that.m_code )
    return true;
  else
    return (m_code == that.m_code) && (m_symbol < that.m_symbol);
} // key_info::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the code of the keyboard key.
 */
bear::input::key_code
bear::input::key_info::get_code() const
{
  return m_code;
} // key_info::get_code()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the translated symbol.
 */
bear::charset::char_type bear::input::key_info::get_symbol() const
{
  return m_symbol;
} // key_info::get_symbol()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is the escape key.
 */
bool bear::input::key_info::is_escape() const
{
  return m_code == keyboard::kc_escape;
} // key_info::is_enter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is an enter key.
 */
bool bear::input::key_info::is_enter() const
{
  return (m_code == keyboard::kc_new_line)
    || (m_code == keyboard::kc_keypad_enter)
    || (m_code == '\r'); // can't find the SDLKey for this one
} // key_info::is_enter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is a tabulation.
 */
bool bear::input::key_info::is_tab() const
{
  return m_code == keyboard::kc_tab;
} // key_info::is_tab()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is a control key.
 */
bool bear::input::key_info::is_control() const
{
  return (m_code == keyboard::kc_left_control)
    || (m_code == keyboard::kc_right_control);
} // key_info::is_control()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is a shift key.
 */
bool bear::input::key_info::is_shift() const
{
  return (m_code == keyboard::kc_left_shift)
    || (m_code == keyboard::kc_right_shift);
} // key_info::is_shift()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is an alt key.
 */
bool bear::input::key_info::is_alt() const
{
  return (m_code == keyboard::kc_left_alt)
    || (m_code == keyboard::kc_right_alt);
} // key_info::is_alt()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is the arrow up key.
 */
bool bear::input::key_info::is_up() const
{
  return m_code == keyboard::kc_up;
} // key_info::is_up()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is the arrow down key.
 */
bool bear::input::key_info::is_down() const
{
  return m_code == keyboard::kc_down;
} // key_info::is_down()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is the arrow left key.
 */
bool bear::input::key_info::is_left() const
{
  return m_code == keyboard::kc_left;
} // key_info::is_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is the arrow right key.
 */
bool bear::input::key_info::is_right() const
{
  return m_code == keyboard::kc_right;
} // key_info::is_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is the home key.
 */
bool bear::input::key_info::is_home() const
{
  return m_code == keyboard::kc_home;
} // key_info::is_home()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is the end key.
 */
bool bear::input::key_info::is_end() const
{
  return m_code == keyboard::kc_end;
} // key_info::is_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is the delete key.
 */
bool bear::input::key_info::is_delete() const
{
  return m_code == keyboard::kc_delete;
} // key_info::is_delete()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is the backspace key.
 */
bool bear::input::key_info::is_backspace() const
{
  return m_code == keyboard::kc_backspace;
} // key_info::is_backspace()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this key is a given function key.
 * \param f The function number.
 */
bool bear::input::key_info::is_function(unsigned int f) const
{
  bool result;

  switch( m_code )
    {
    case keyboard::kc_F1: result = (f == 1); break;
    case keyboard::kc_F2: result = (f == 2); break;
    case keyboard::kc_F3: result = (f == 3); break;
    case keyboard::kc_F4: result = (f == 4); break;
    case keyboard::kc_F5: result = (f == 5); break;
    case keyboard::kc_F6: result = (f == 6); break;
    case keyboard::kc_F7: result = (f == 7); break;
    case keyboard::kc_F8: result = (f == 8); break;
    case keyboard::kc_F9: result = (f == 9); break;
    case keyboard::kc_F10: result = (f == 10); break;
    case keyboard::kc_F11: result = (f == 11); break;
    case keyboard::kc_F12: result = (f == 12); break;
    case keyboard::kc_F13: result = (f == 13); break;
    case keyboard::kc_F14: result = (f == 14); break;
    case keyboard::kc_F15: result = (f == 15); break;
    default:
      result = false;
    }

  return result;
} // key_info::is_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the key represents a printable symbol.
 */
bool bear::input::key_info::is_printable() const
{
  return !(m_symbol < 0x20)       /* unicode "space" */
    && ( (m_symbol < 0x7F)        /* unicode "delete" */
         || ( m_symbol > 0x9F ) ) /* unicode ??? */
    && ( m_symbol != 0xAD );      /* unicode ??? */
} // key_info::is_printable()
