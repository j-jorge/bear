/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class to give informations about the sizes (characters, on screen)
 *        of a text.
 * \author Julien Jorge
 */
#ifndef __VISUAL_TEXT_METRIC_HPP__
#define __VISUAL_TEXT_METRIC_HPP__

#include "visual/font/font.hpp"

#include "visual/class_export.hpp"

#include <string>
#include <claw/math.hpp>

namespace bear
{
  namespace visual
  {
    /**
     * \brief A class to give informations about the sizes (characters, on
     *        screen) of a text.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT text_metric
    {
    public:
      text_metric
        ( const std::string& text, const font& f );

      size_type width() const;
      size_type height() const;

    private:
      /** \brief The size of the text in pixel units. */
      size_box_type m_pixel_size;

    }; // class text_metric
  } // namespace visual
} // namespace bear

#endif // __VISUAL_TEXT_METRIC_HPP__
