/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the gui::button class.
 * \author Julien Jorge
 */
#include "gui/button.hpp"

#include "gui/static_text.hpp"
#include "gui/picture.hpp"
#include "input/keyboard.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The font used to display the text.
 * \param label The text displayed in the button.
 */
bear::gui::button::button( const font_type& f, const std::string& label )
  : visual_component(), m_text( NULL ), m_icon( NULL ), m_margin(0)
{
  create();
  m_text->set_font(f);
  m_text->set_text(label);
  fit(m_margin);
} // button::button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The font used to display the text.
 * \param label The text displayed in the button.
 * \param c The function called when the button is activated.
 */
bear::gui::button::button
( const font_type& f, const std::string& label, const callback& c )
  : visual_component(), m_text( NULL ), m_icon( NULL ), m_margin(0)
{
  m_click_callback.add(c);

  create();
  m_text->set_font(f);
  m_text->set_text(label);
  fit(m_margin);
} // button::button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs a button with an icon and no text.
 * \param icon The icon.
 */
bear::gui::button::button( const visual::sprite& icon )
  : visual_component(), m_text( NULL ), m_icon( NULL ), m_margin(0)
{
  create();

  m_icon->set_size( icon.get_size() );
  m_icon->set_picture( icon );
  fit(m_margin);
} // button::button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the font of the text.
 * \param f The new font.
 */
void bear::gui::button::set_font( font_type f )
{
  set_size_maximum();
  m_text->set_font(f);

  adjust_component_positions();
} // button::set_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the margin around the text of the control.
 * \param m The margin.
 */
void bear::gui::button::set_margin( size_type m )
{
  m_margin = m;

  adjust_component_positions();
} // button::set_margin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the text of the control.
 * \param text The new text.
 */
void bear::gui::button::set_text( const std::string& text )
{
  set_size_maximum();
  m_text->set_text(text);

  adjust_component_positions();
} // button::set_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the text of the control.
 */
const std::string& bear::gui::button::get_text() const
{
  return m_text->get_text();
} // button::get_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the icon of the control.
 * \param icon The new icon.
 */
void bear::gui::button::set_icon( const visual::sprite& icon )
{
  set_size_maximum();

  m_icon->set_size( icon.get_size() );
  m_icon->set_picture( icon );

  adjust_component_positions();
} // button::set_icon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a callback called when the button is clicked.
 * \param c The callback.
 */
void bear::gui::button::add_callback( const callback& c )
{
  m_click_callback.add(c);
} // button::add_callback()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the control.
 */
void bear::gui::button::create()
{
  set_size_maximum();

  m_text = new static_text();

  insert( m_text );
  m_text->set_auto_size(true);

  m_icon = new picture();
  insert( m_icon );
} // button::create()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjusts the placement of the icon and the label.
 */
void bear::gui::button::adjust_component_positions()
{
  set_size_maximum();
  
  const size_type h = std::max( m_text->height(), m_icon->height() );

  m_icon->set_left( 0 );
  m_icon->set_bottom( ( h - m_icon->height() ) / 2 );

  if ( m_text->get_text().empty() )
    m_text->set_left( 0 );
  else
    m_text->set_left( m_icon->right() + m_margin );

  m_text->set_bottom( ( h - m_text->height() ) / 2 );
  
  fit(m_margin);
} // button::adjust_component_positions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that a key has been pressed.
 * \param key The code of the key.
 */
bool bear::gui::button::on_key_press( const bear::input::key_info& key )
{
  bool result = true;

  if ( key.is_enter() || (key.get_code() == input::keyboard::kc_space) )
    m_click_callback.execute();
  else
    result = false;

  return result;
} // button::on_key_press()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that a joystick button has been pressed.
 * \param joy_button The code of the button.
 * \param joy_index The index of the joytick.
 */
bool bear::gui::button::on_button_press
( bear::input::joystick::joy_code joy_button, unsigned int joy_index )
{
  bool result = true;

  switch( joy_button )
    {
    case bear::input::joystick::jc_button_1:
    case bear::input::joystick::jc_button_2:
    case bear::input::joystick::jc_button_3:
    case bear::input::joystick::jc_button_4:
    case bear::input::joystick::jc_button_5:
    case bear::input::joystick::jc_button_6:
    case bear::input::joystick::jc_button_7:
    case bear::input::joystick::jc_button_8:
    case bear::input::joystick::jc_button_9:
    case bear::input::joystick::jc_button_10:
    case bear::input::joystick::jc_button_11:
    case bear::input::joystick::jc_button_12:
    case bear::input::joystick::jc_button_13:
    case bear::input::joystick::jc_button_14:
    case bear::input::joystick::jc_button_15:
    case bear::input::joystick::jc_button_16:
      m_click_callback.execute();
      break;
    default:
      result = false;
    }

  return result;
} // button::on_button_press()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that a mouse button has been pressed.
 * \param key The code of the button.
 * \param pos The position of the mouse.
 */
bool bear::gui::button::on_mouse_press
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  m_click_callback.execute();
  return true;
} // button::on_mouse_press()
