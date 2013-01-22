/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::gui::static_text class.
 * \author Julien Jorge
 */
#include "gui/static_text.hpp"

#include "visual/scene_writing.hpp"
#include "visual/text_metric.hpp"
#include "visual/text_layout.hpp"
#include "visual/text_layout_display_size.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param result (out) The length of the longest text.
 *
 * The instance keep the references on each parameter. So they must live longen
 * than \a this.
 */
bear::gui::static_text::arrange_longest_text::arrange_longest_text
( std::size_t& result )
  : m_result(result)
{

} // static_text::arrange_longest_text::arrange_longest_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the index of the last character displayed.
 * \param p The position where the word starts.
 * \param first The first character of the word to display.
 * \param last The character just past the last character to display.
 */
void bear::gui::static_text::arrange_longest_text::operator()
  ( position_type p, std::size_t first, std::size_t last )
{
  m_result = last;
} // static_text::arrange_longest_text::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::gui::static_text::static_text()
  : visual_component(), m_auto_size(false), m_margin(1, 1)
{
  set_size(m_margin);
} // static_text::static_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The font used to draw the text.
 */
bear::gui::static_text::static_text( font_type f )
  : visual_component(), m_font(f), m_auto_size(false), m_margin(1, 1)
{

} // static_text::static_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the Font used to draw the text.
 * \param b The new value.
 */
void bear::gui::static_text::set_font( font_type f )
{
  m_font = f;

  if (m_auto_size)
    adjust_size_to_text();

  refresh_writing();
} // static_text::set_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the control must adjust its size to fit the text.
 * \param b The new value.
 */
void bear::gui::static_text::set_auto_size( bool b )
{
  m_auto_size = b;

  if (m_auto_size)
    adjust_size_to_text();
} // static_text::set_auto_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the text from the component.
 */
void bear::gui::static_text::clear()
{
  set_text( std::string() );
} // static_text::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the text to draw.
 * \param text The text to draw.
 */
void bear::gui::static_text::set_text( const std::string& text )
{
  m_text = text;

  if (m_auto_size)
    adjust_size_to_text();

  refresh_writing();
} // static_text::set_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the text.
 */
const std::string& bear::gui::static_text::get_text() const
{
  return m_text;
} // static_text::get_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the font used for displaying the text.
 */
bear::gui::static_text::font_type bear::gui::static_text::get_font() const
{
  return m_font;
} // static_text::get_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjusts the height to fit the text.
 */
void bear::gui::static_text::expand_vertically()
{
  const size_box_type s
    ( width() - 2 * m_margin.x,
      m_font.get_line_spacing() * m_text.length() );

  visual::text_layout_display_size func( m_text, m_font, s.y );
  visual::text_layout layout
    ( m_font, m_text, s, visual::text_align::align_left );

  layout.arrange_text<visual::text_layout_display_size&>( func );

  set_size( func.get_bounding_box().size() + 2 * m_margin );
} // static_text::expand_vertically()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the length of the longest text that can be displayed in this
 *        component.
 * \param text The text on which we work.
 * \param i The position in \a text
 * \return the position of the first character out of the component.
 */
std::size_t bear::gui::static_text::get_longest_text
( const std::string& text, std::size_t i ) const
{
  std::size_t result;

  arrange_longest_text func(result);
  visual::text_layout layout( m_font, text, get_size() - 2 * m_margin );

  layout.arrange_text( func );

  return result;
} // static_text::get_longest_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the margins between the text and the borders of the control.
 * \param x The margin on the x-axis.
 * \param y The margin on the y-axis.
 */
void bear::gui::static_text::set_margin( coordinate_type x, coordinate_type y )
{
  m_margin.x = x;
  m_margin.y = y;
} // static_text::set_margin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the margins between the text and the borders of the control.
 * \param m The margins.
 */
void bear::gui::static_text::set_margin( const size_box_type& m )
{
  m_margin = m;
} // static_text::set_margin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the minimal height required to display one line of text.
 */
bear::gui::size_type bear::gui::static_text::get_min_height_with_text() const
{
  return 2 * m_margin.y + m_font.get_line_spacing();
} // static_text::get_min_height_with_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the rendering attributes on text.
 */
bear::visual::bitmap_rendering_attributes&
bear::gui::static_text::get_text_rendering_attributes()
{
  return m_text_rendering_attributes;
} // static_text::get_text_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the rendering attributes on text.
 */
const bear::visual::bitmap_rendering_attributes&
bear::gui::static_text::get_text_rendering_attributes() const
{
  return m_text_rendering_attributes;
} // static_text::get_text_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the scene elements of the component.
 * \param e (out) The scene elements.
 */
void bear::gui::static_text::display
( std::list<visual::scene_element>& e ) const
{
  visual::scene_writing new_e
    (left() + m_margin.x, bottom() + m_margin.y, m_writing);
  new_e.get_rendering_attributes().combine(m_text_rendering_attributes);

  e.push_back( new_e );
} // static_text::display()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the size of the component to fit the text.
 */
void bear::gui::static_text::adjust_size_to_text()
{
  visual::text_metric tm( m_text, m_font );
  size_box_type size( tm.width(), tm.height() );
  set_size( size + 2 * m_margin );
} // static_text::adjust_size_to_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when the size of the component has changed.
 */
void bear::gui::static_text::on_resized()
{
  bool invalid_size(false);
  size_box_type s(get_size());

  if ( width() < 2 * m_margin.x )
    {
      s.x = 2 * m_margin.x;
      invalid_size = true;
    }
  if ( height() < 2 * m_margin.y )
    {
      s.y = 2 * m_margin.y;
      invalid_size = true;
    }

  if (invalid_size)
    set_size(s);
  else
    refresh_writing();
} // static_text::on_resized()

/*----------------------------------------------------------------------------*/
/**
 * \brief Recreate m_writing.
 */
void bear::gui::static_text::refresh_writing()
{
  m_writing.create( m_font, m_text, get_size() - 2 * m_margin );
} // static_text::refresh_writing()
