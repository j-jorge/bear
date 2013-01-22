/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::text_layout_display_size class.
 * \author Julien Jorge
 */
#include "visual/text_layout_display_size.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param text The text to arrange.
 * \param f The font used to display the text.
 * \param top The top position of the box where the text is displayed.
 */
bear::visual::text_layout_display_size::text_layout_display_size
( std::string text, visual::font f, coordinate_type top )
  : m_text(text), m_font(f), m_bounding_box(0, top, 0, top),
    m_bounding_box_initialized(false)
{

} // text_layout_display_size::text_layout_display_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the maximum size.
 * \param p The position where the word starts.
 * \param first The first character of the word to display.
 * \param last The character just past the last character to display.
 */
void bear::visual::text_layout_display_size::operator()
  ( position_type p, std::size_t first, std::size_t last )
{
  if ( !m_bounding_box_initialized )
    {
      m_bounding_box =
        rectangle_type( p.x, m_bounding_box.top(), p.x, m_bounding_box.top() );
      m_bounding_box_initialized = true;
    }

  m_bounding_box = m_bounding_box.join( rectangle_type(p, p) );

  position_type baseline_position(p);

  for ( ; first!=last; ++first )
    {
      const glyph_metrics m( m_font.get_metrics( m_text[first] ) );
      const sprite spr( m_font.get_sprite( m_text[first] ) );

      const rectangle_type glyph_box
        ( baseline_position.x,
          baseline_position.y + m.get_bearing().y,
          baseline_position.x + std::max( m.get_advance().x, spr.width() ),
          baseline_position.y + m.get_bearing().y + spr.height() );

      m_bounding_box = m_bounding_box.join( glyph_box );
      baseline_position.x += m.get_advance().x;
    }
} // text_layout_display_size::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the computed bottom right position of the displayed text.
 */
bear::visual::rectangle_type
bear::visual::text_layout_display_size::get_bounding_box() const
{
  return m_bounding_box;
} // text_layout_display_size::get_bounding_box()
