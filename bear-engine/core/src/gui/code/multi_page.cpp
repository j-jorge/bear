/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::gui::multi_page class.
 * \author Julien Jorge
 */
#include "gui/multi_page.hpp"

#include "gui/static_text.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The font used to draw the text.
 */
bear::gui::multi_page::multi_page( font_type f )
  : visual_component(), m_current_mark(0)
{
  m_static_text = new static_text(f);
  m_dots_text = new static_text(f);

  insert( m_static_text );
  insert( m_dots_text );

  m_dots_text->set_auto_size(true);
  m_dots_text->set_text("[...]");
  m_dots_text->set_visible(false);

  set_text("");
} // multi_page::multi_page()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show the previous page of text.
 */
void bear::gui::multi_page::previous()
{
  if ( m_current_mark != 0 )
    {
      --m_current_mark;
      set_static_text();
    }
} // multi_page::previous()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show the next page of text.
 */
void bear::gui::multi_page::next()
{
  if (m_bookmark.size() > 1)
    if ( m_current_mark + 1 < m_bookmark.size() )
      {
        ++m_current_mark;
        set_static_text();
      }
} // multi_page::next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the text to show.
 * \param text the new text.
 */
void bear::gui::multi_page::set_text( const std::string& text )
{
  m_text = text;

  create_indices();

  m_current_mark = 0;

  set_static_text();
} // multi_page::set_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust sub components when the size is changed.
 */
void bear::gui::multi_page::on_resized()
{
  size_box_type size;
  position_type pos(0, 0);
  const size_type font_height
    ( m_static_text->get_font().get_line_spacing() );

  size.x = width();

  // be sure that we can see the longest text possible
  m_dots_text->set_position(pos);
  m_dots_text->set_auto_size(true);

  if ( width() > m_dots_text->width() )
    pos.x = width() - m_dots_text->width();

  if ( height() >= 2 * font_height )
    {
      size.y = height() - font_height;
      m_static_text->set_size( size );

      size.y = font_height;
      pos.y = height() - size.y;
      m_dots_text->set_position( pos );
      m_dots_text->set_size( size );
    }
  else
    {
      size.y = std::min( font_height, height() );
      pos.y = height() - size.y;
      m_dots_text->set_position( pos );
      m_dots_text->set_size( size );

      size.y = height() - size.y;
      m_static_text->set_size( size );
    }

  create_indices();
  m_current_mark = 0;
  set_static_text();
} // multi_page::on_resized()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the text displayed in the static_text and move m_current_mark.
 */
void bear::gui::multi_page::set_static_text()
{
  if ( m_current_mark + 1 != m_bookmark.size() )
    {
      m_static_text->set_text( std::string( m_bookmark[m_current_mark],
                                            m_bookmark[m_current_mark + 1] ) );

      m_dots_text->set_visible( m_bookmark[m_current_mark+1] != m_text.end() );
    }
} // multi_page::set_static_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the iterators on each page of the text.
 */
void bear::gui::multi_page::create_indices()
{
  m_bookmark.clear();

  std::string::const_iterator it = m_text.begin();

  m_bookmark.push_back( it );

  while ( it != m_text.end() )
    {
      it += m_static_text->get_longest_text( m_text, it - m_text.begin() );
      m_bookmark.push_back( it );
    }
} // multi_page::create_indices()
