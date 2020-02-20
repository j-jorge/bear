/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::text_metric class.
 * \author Julien Jorge
 */
#include "bear/visual/text_metric.hpp"

#include "bear/visual/text_align.hpp"
#include "bear/visual/text_layout.hpp"
#include "bear/visual/text_layout_display_size.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param text The text to measure.
 * \param f The font used to print the text.
 */
bear::visual::text_metric::text_metric( const std::string& text, const font& f )
  : m_pixel_size(0, 0)
{
  const size_type height( text.length() * f.get_line_spacing() );
  const size_type max_size( std::numeric_limits<size_type>::max() );

  text_layout_display_size func( text, f, height );
  text_layout layout
    ( f, text, size_box_type( max_size, height ), text_align::align_left );

  layout.arrange_text<text_layout_display_size&>( func );

  m_pixel_size = func.get_bounding_box().size();
} // text_metric::text_metric()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of the text, in pixels.
 */
bear::visual::size_type bear::visual::text_metric::width() const
{
  return m_pixel_size.x;
} // text_metric::width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of the text, in pixels.
 */
bear::visual::size_type bear::visual::text_metric::height() const
{
  return m_pixel_size.y;
} // text_metric::height()
