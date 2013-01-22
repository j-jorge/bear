/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A component containing multiple pages of text.
 * \author Julien Jorge
 */
#ifndef __GUI_MULTI_PAGE_HPP__
#define __GUI_MULTI_PAGE_HPP__

#include "gui/visual_component.hpp"

#include "visual/font/font.hpp"
#include "gui/class_export.hpp"

#include <vector>

namespace bear
{
  namespace gui
  {
    class static_text;

    /**
     * \brief A component containing multiple pages of text.
     * \author Julien Jorge
     */
    class GUI_EXPORT multi_page:
      public visual_component
    {
    public:
      /** \brief The type of the font used to display the text. */
      typedef visual::font font_type;

    public:
      explicit multi_page( font_type f );

      void previous();
      void next();

      void set_text( const std::string& text );

    private:
      void on_resized();
      void set_static_text();
      void create_indices();

    private:
      /** \brief The text to display. */
      std::string m_text;

      /** \brief Positions, in the text, of each page. */
      std::vector<std::string::const_iterator> m_bookmark;

      /** \brief Current page of text. */
      unsigned int m_current_mark;

      /** \brief Component used to display the text. */
      static_text* m_static_text;

      /** \brief Component used to display the "..." text when there's more text
          to display. */
      static_text* m_dots_text;

    }; // class multi_page
  } // namespace gui
} // namespace bear

#endif // __GUI_MULTI_PAGE_HPP__
