/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::with_text class.
 * \author Julien Jorge
 */
#include "engine/item_brick/with_text.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::with_text::with_text()
  : m_horizontal_align( visual::text_align::align_left ),
    m_vertical_align( visual::text_align::align_top )
{

} // with_text::with_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the text of the item.
 * \param text The text.
 */
void bear::engine::with_text::set_text( const std::string& text )
{
  m_text = text;
  refresh_writing();
} // with_text::set_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the text of the item.
 */
const std::string& bear::engine::with_text::get_text() const
{
  return m_text;
} // with_text::get_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the font of the text.
 * \param f The font.
 */
void bear::engine::with_text::set_font( const visual::font& f )
{
  m_font = f;
  refresh_writing();
} // with_text::set_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the font of the text.
 */
const bear::visual::font& bear::engine::with_text::get_font() const
{
  return m_font;
} // with_text::get_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the horizontal alignment of the text.
 * \param a The horizontal alignment.
 */
void bear::engine::with_text::set_horizontal_align
( visual::text_align::horizontal_align a )
{
  m_horizontal_align = a;
  refresh_writing();
} // with_text::set_horizontal_align()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the horizontal alignment of the text.
 */
bear::visual::text_align::horizontal_align
bear::engine::with_text::get_horizontal_align() const
{
  return m_horizontal_align;
} // with_text::get_horizontal_align()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the vertical alignment of the text.
 * \param a The vertical alignment.
 */
void bear::engine::with_text::set_vertical_align
( visual::text_align::vertical_align a )
{
  m_vertical_align = a;
  refresh_writing();
} // with_text::set_vertical_align()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the vertical alignment of the text.
 */
bear::visual::text_align::vertical_align
bear::engine::with_text::get_vertical_align() const
{
  return m_vertical_align;
} // with_text::get_vertical_align()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the writing of the text with the font.
 */
const bear::visual::writing& bear::engine::with_text::get_writing() const
{
  return m_writing;
} // with_text::get_writing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fit the text in a box.
 * \param s The bounds of the text.
 */
void bear::engine::with_text::fit_in_box( const universe::size_box_type& s )
{
  m_writing.create( m_font, m_text, s, m_horizontal_align, m_vertical_align );
} // with_text::fit_in_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Refresh the content of the writing.
 */
void bear::engine::with_text::refresh_writing()
{
  m_writing.create( m_font, m_text, m_horizontal_align, m_vertical_align );
} // with_text::refresh_writing()
