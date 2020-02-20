/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::glyph_metrics class.
 * \author Julien Jorge
 */
#include "bear/visual/font/glyph_metrics.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs the metrics with no dimension. The advance and bearing will
 *        both be equal to zero.
 */
bear::visual::glyph_metrics::glyph_metrics()
  : m_advance( 0, 0 ), m_bearing( 0, 0 )
{

} // glyph_metrics::glyph_metrics()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param advance How far to move the cursor when the glyph is displayed, for
 *        the horizontal and vertical layouts.
 * \param bearing The distance between the origin on the baseline and the bottom
 *        left corner of the glyph.
 */
bear::visual::glyph_metrics::glyph_metrics
( size_box_type advance, size_box_type bearing )
  : m_advance( advance ), m_bearing( bearing )
{

} // glyph_metrics::glyph_metrics()

/*----------------------------------------------------------------------------*/
/**
 * \brief How far to move the cursor when the glyph is displayed.
 * \return The distance for the horizontal and vertical layouts.
 */
bear::visual::size_box_type bear::visual::glyph_metrics::get_advance() const
{
  return m_advance;
} // glyph_metrics::get_advance()

/*----------------------------------------------------------------------------*/
/**
 * \brief The distance between the origin on the baseline and the bottom
 *        left corner of the glyph.
 */
bear::visual::size_box_type bear::visual::glyph_metrics::get_bearing() const
{
  return m_bearing;
} // glyph_metrics::get_bearing()
