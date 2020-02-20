/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The glyph metrics class describes how a glyph must be placed when
 *        displayed.
 * \author Julien Jorge
 */
#ifndef __VISUAL_GLYPH_METRICS_HPP__
#define __VISUAL_GLYPH_METRICS_HPP__

#include "bear/visual/types.hpp"

#include "bear/visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief The glyph metrics class describes how a glyph must be placed when
     *        displayed.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT glyph_metrics
    {
    public:
      glyph_metrics();
      glyph_metrics( size_box_type advance, size_box_type bearing );

      size_box_type get_advance() const;
      size_box_type get_bearing() const;

    private:
      /** \brief How far to move the cursor when the glyph is displayed. The
          variable contains the distance for the horizontal and vertical
          layouts. */
      size_box_type m_advance;

      /** \brief The distance between the origin on the baseline and the bottom
          left corner of the glyph. */
      size_box_type m_bearing;

    }; // class glyph_metrics

  } // namespace visual
} // namespace bear

#endif // __VISUAL_GLYPH_METRICS_HPP__
