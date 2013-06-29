/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to give informations about the sizes (characters, on screen)
 *        of a text.
 * \author Julien Jorge
 */
#ifndef __VISUAL_TEXT_LAYOUT_DISPLAY_SIZE_HPP__
#define __VISUAL_TEXT_LAYOUT_DISPLAY_SIZE_HPP__

#include <string>
#include "visual/font/font.hpp"

#include "visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief The class passed to text_layout to compute the size of the
     *        text when displayed.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT text_layout_display_size
    {
    public:
      text_layout_display_size
        ( std::string text, visual::font f, coordinate_type top );

      void operator()
        ( position_type p, std::size_t first, std::size_t last );

      rectangle_type get_bounding_box() const;

    private:
      /** \brief The text to arrange. */
      std::string const m_text;

      /** \brief The font used to display the text. */
      visual::font const m_font;

      /** \brief The bounding box of the displayed text. */
      rectangle_type m_bounding_box;

      /** \brief Tells if m_bounding_box has already received a value. */
      bool m_bounding_box_initialized;

    }; // class text_layout_display_size

  } // namespace visual
} // namespace bear

#endif // __VISUAL_TEXT_LAYOUT_DISPLAY_SIZE_HPP__
