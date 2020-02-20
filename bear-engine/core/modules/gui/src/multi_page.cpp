/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::gui::multi_page class.
 * \author Julien Jorge
 */
#include "bear/gui/multi_page.hpp"

#include "bear/gui/static_text.hpp"

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

  m_dots_text->set_auto_size(true);
  m_dots_text->set_text("[...]");
  m_dots_text->set_visible(false);

  // The multipage must at least contain the dots. */
  set_size( m_dots_text->get_size() );

  insert( m_static_text );
  insert( m_dots_text );

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
  const bool large_enough
    ( ( width() >= m_dots_text->width() )
      && ( height() >= 2 * m_dots_text->height() ) );

  m_dots_text->set_visible( large_enough );
  m_static_text->set_visible( large_enough );

  if ( large_enough )
    {
      m_dots_text->set_position
        ( position_type( width() - m_dots_text->width() - 1, 1 ) );

      m_static_text->set_size
        ( size_box_type( width(), height() - m_dots_text->height() ) );
      m_static_text->set_position( position_type( 0, m_dots_text->height() ) );
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
      const std::string text( it, std::string::const_iterator(m_text.end()) );
      const std::size_t text_length( m_static_text->get_longest_text( text ) );

      if ( text_length == 0 )
        break;

      const std::string::size_type next_position
        ( m_text.find_first_not_of
          ( ' ', (it - m_text.begin()) + text_length ) );

      if ( next_position == std::string::npos )
        it = m_text.end();
      else
        it = m_text.begin() + next_position;

      m_bookmark.push_back( it );
    }
} // multi_page::create_indices()
