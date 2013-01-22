/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The different alignment modes of the text.
 * \author Julien Jorge
 */
#ifndef __VISUAL_TEXT_ALIGN_HPP__
#define __VISUAL_TEXT_ALIGN_HPP__

#include <string>

#include "visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief The different alignment modes of the text.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT text_align
    {
    public:
      /**
       * \brief The horizontal alignment modes.
       */
      enum horizontal_align
      {
        /** \brief Align the lines of text of the left side. */
        align_left,

        /** \brief Align the center of the lines of text. */
        align_center,

        /** \brief Align the lines of text of the right side. */
        align_right

      }; // enum horizontal

      /**
       * \brief The vertical alignment modes.
       */
      enum vertical_align
      {
        /** \brief Align the lines of text of the bottom side. */
        align_bottom,

        /** \brief Align the lines of text in the middle. */
        align_middle,

        /** \brief Align the lines of text of the top side. */
        align_top

      }; // enum vertical

    public:
      static horizontal_align horizontal_align_from_string
        ( std::string s, horizontal_align default_value );
      static vertical_align vertical_align_from_string
        ( std::string s, vertical_align default_value );

    }; // class text_align
  } // namespace visual
} // namespace bear

#endif // __VISUAL_TEXT_ALIGN_HPP__
