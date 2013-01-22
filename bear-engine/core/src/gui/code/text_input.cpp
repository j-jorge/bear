/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::gui::text_input class.
 * \author Julien Jorge
 */
#include "gui/text_input.hpp"

#include "gui/static_text.hpp"
#include "visual/scene_line.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The font used to draw the text.
 * \param cursor_color The color of the cursor.
 */
bear::gui::text_input::text_input
( font_type f, visual::color_type cursor_color )
  : visual_component(), m_cursor(0), m_cursor_color(cursor_color), m_first(0),
    m_last(0), m_line_length(0)
{
  m_static_text = new static_text(f);
  insert(m_static_text);
} // text_input::text_input()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the text from the control.
 */
void bear::gui::text_input::clear()
{
  set_text( std::string() );
} // text_input::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the text of the component.
 * \param text The new text.
 */
void bear::gui::text_input::set_text( const std::string& text )
{
  m_text = text;
  m_cursor = m_last = m_text.size();
  m_first = m_last - std::min( m_text.size(), m_line_length-1 );

  adjust_visible_part_of_text();
} // text_input::set_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the text of the component.
 */
const std::string& bear::gui::text_input::get_text() const
{
  return m_text;
} // text_input::get_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add callback called when enter is pressed.
 * \param c The callback.
 */
void bear::gui::text_input::add_enter_callback( const callback& c )
{
  m_enter_callback.add(c);
} // text_input::add_enter_callback()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert a key in the text.
 * \param key The pressed key.
 */
bool bear::gui::text_input::on_key_press( const input::key_info& key )
{
  bool result = true;

  if ( !key.is_printable() )
    result = special_code( key );

  if (result)
    adjust_visible_part_of_text();

  return result;
} // text_input::on_key_press()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert a character in the text.
 * \param key The pressed key.
 */
bool bear::gui::text_input::on_char_pressed( const input::key_info& key )
{
  if ( key.is_printable() )
    {
      insert_character( key.get_symbol() );
      adjust_visible_part_of_text();
    }

  return true;
} // text_input::on_char_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Force the height to be a little higher that the font size.
 */
void bear::gui::text_input::on_resized()
{
  const size_type default_height( m_static_text->get_min_height_with_text() );

  set_size( width(), default_height );
  m_static_text->set_size( width(), default_height );

  m_line_length =
    m_static_text->width()
    / m_static_text->get_font().get_metrics('m').get_advance().x;
} // text_input::on_resized()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the scene elements of the component.
 * \param e (out) The scene elements.
 */
void bear::gui::text_input::display( std::list<visual::scene_element>& e ) const
{
  std::vector<visual::position_type> p(2);

  p[0].x = 0;
  p[0].y = 0;
  p[1].y = height();

  for (std::size_t i(m_first); i!=m_cursor; ++i)
    p[0].x +=
      m_static_text->get_font().get_metrics( m_text[i] ).get_advance().x;

  p[1].x = p[0].x;

  e.push_back( visual::scene_line(left(), bottom(), m_cursor_color, p, 1) );
} // text_input::display()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert a character at cursor position.
 * \param key The character to insert.
 */
void bear::gui::text_input::insert_character( char key )
{
  m_text.insert( m_cursor, 1, key );

  if ( m_text.size() < m_line_length )
    ++m_last;

  move_right();
} // text_input::insert_character()

/*----------------------------------------------------------------------------*/
/**
 * \brief Process a special key code.
 * \param key The code of the key.
 * \return True if the code has been processed.
 */
bool bear::gui::text_input::special_code( const input::key_info& key )
{
  bool result = true;

  if( key.is_delete() )
    {
      if (m_cursor < m_text.size() )
        {
          m_text.erase(m_cursor, 1);

          if (m_last == m_text.size() + 1)
            --m_last;
        }
    }
  else if ( key.is_backspace() )
    {
      if (m_cursor > 0)
        {
          m_text.erase(m_cursor - 1, 1);

          if (m_last == m_text.size())
            --m_last;

          move_left();
        }
    }
  else if ( key.is_left() )
    move_left();
  else if ( key.is_right() )
    move_right();
  else if ( key.is_home() )
    {
      m_cursor = 0;
      adjust_text_by_left();
    }
  else if ( key.is_end() )
    {
      m_cursor = m_text.size();
      adjust_text_by_right();
    }
  else if ( key.is_enter() && !m_enter_callback.empty() )
    m_enter_callback.execute();
  else
    result = false;

  return result;
} // text_input::special_code()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the cursor one character to the left.
 */
void bear::gui::text_input::move_left()
{
  if (m_cursor > 0)
    {
      --m_cursor;

      adjust_text_by_left();
    }
} // text_input::move_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the cursor one character to the right.
 */
void bear::gui::text_input::move_right()
{
  if (m_cursor < m_text.size())
    {
      ++m_cursor;

      adjust_text_by_right();
    }
} // text_input::move_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the part of the visible text when the cursor is before the
 *        begining.
 */
void bear::gui::text_input::adjust_text_by_left()
{
  if (m_cursor < m_first)
    {
      m_first = m_cursor;
      m_last = m_first + std::min( m_text.size() - m_first, m_line_length-1 );
    }
} // text_input::adjust_text_by_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the part of the visible text when the cursor is after the end.
 */
void bear::gui::text_input::adjust_text_by_right()
{
  if (m_cursor > m_last)
    {
      m_last = m_cursor;
      m_first = m_last - std::min( m_text.size(), m_line_length-1 );
    }
} // text_input::adjust_text_by_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the visible text according to cursor's position.
 */
void bear::gui::text_input::adjust_visible_part_of_text()
{
  m_static_text->set_text( std::string(m_text, m_first, m_last - m_first) );
} // text_input::adjust_visible_part_of_text()
