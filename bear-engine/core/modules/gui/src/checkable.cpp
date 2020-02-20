/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the gui::checkable class.
 * \author Julien Jorge
 */
#include "bear/gui/checkable.hpp"

#include "bear/gui/static_text.hpp"
#include "bear/input/keyboard.hpp"
#include "bear/visual/scene_sprite.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param off The sprite displayed when the box is not checked.
 * \param on The sprite displayed when the box is checked.
 */
bear::gui::checkable::checkable
( const visual::sprite& off, const visual::sprite& on )
  : visual_component(), m_text(NULL), m_checked(false), m_off(off), m_on(on)
{
  create();
} // checkable::checkable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param off The sprite displayed when the box is not checked.
 * \param on The sprite displayed when the box is checked.
 * \param f The font used to display the text.
 */
bear::gui::checkable::checkable
( const visual::sprite& off, const visual::sprite& on, font_type f )
  : visual_component(), m_text(NULL), m_checked(false), m_off(off), m_on(on)
{
  create();
  m_text->set_font(f);
} // checkable::checkable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the font of the text.
 * \param f The new font.
 */
void bear::gui::checkable::set_font( font_type f )
{
  set_size_maximum();
  m_text->set_font(f);

  adjust_text_position();

  fit();
} // checkable::set_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the text of the control.
 * \param text The new text.
 */
void bear::gui::checkable::set_text( const std::string& text )
{
  set_size_maximum();
  m_text->set_text(text);

  adjust_text_position();

  fit();
} // checkable::set_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check/uncheck the box.
 * \param b Tell if the box is checked or not.
 */
void bear::gui::checkable::check( bool b )
{
  if ( b == m_checked )
    return;

  set_value(b);

  if ( m_checked )
    m_checked_callback.execute();
  else
    m_unchecked_callback.execute();
} // checkable::check()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the value of the box.
 */
void bear::gui::checkable::toggle_value()
{
  check( !checked() );
} // checkable::toggle_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value, without calling the callbacks.
 */
void bear::gui::checkable::set_value( bool b )
{
  m_checked = b;
} // checkable::set_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the text of the control.
 */
const std::string& bear::gui::checkable::get_text() const
{
  return m_text->get_text();
} // checkable::get_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the box is checked or not.
 */
bool bear::gui::checkable::checked() const
{
  return m_checked;
} // checkable::checked()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a callback called when the box becomes checked.
 * \param c The callback.
 */
void bear::gui::checkable::add_checked_callback( const callback& c )
{
  m_checked_callback.add(c);
} // checkable::add_checked_callback()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a callback called when the box becomes unchecked.
 * \param c The callback.
 */
void bear::gui::checkable::add_unchecked_callback( const callback& c )
{
  m_unchecked_callback.add(c);
} // checkable::add_unchecked_callback()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the scene elements of the component.
 * \param e (out) The scene elements.
 */
void bear::gui::checkable::display( std::list<visual::scene_element>& e ) const
{
  position_type p( bottom_left() );

  const coordinate_type c_x( std::max( m_on.width(), m_off.width() ) / 2 );

  if (m_checked)
    {
      p.x += c_x - m_on.width() / 2;
      p.y += (height() - m_on.height()) / 2;
      e.push_back( visual::scene_sprite(p.x, p.y, m_on) );
    }
  else
    {
      p.x += c_x - m_off.width() / 2;
      p.y += (height() - m_off.height()) / 2;
      e.push_back( visual::scene_sprite(p.x, p.y, m_off) );
    }
} // checkable::display()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the control.
 */
void bear::gui::checkable::create()
{
  set_size_maximum();

  m_text = new static_text();
  insert(m_text);
  m_text->set_auto_size(true);

  adjust_text_position();

  fit();
} // checkable::create()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjusts the size of the control to the size of its sub controls.
 */
void bear::gui::checkable::fit()
{
  size_type w( std::max(m_on.width(), m_off.width()) );

  if ( !m_text->get_text().empty() )
    w += m_text->width() + 5;

  set_size
    ( w,
      std::max( std::max(m_on.height(), m_off.height()), m_text->height() ) );
} // checkable::fit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjusts the position of the text.
 */
void bear::gui::checkable::adjust_text_position()
{
  m_text->set_position( std::max(m_off.width(), m_on.width() ) + 5, 0 );
} // checkable::adjust_text_position()
