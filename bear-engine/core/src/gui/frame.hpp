/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A frame: a box with a border and a background.
 * \author Julien Jorge
 */
#ifndef __GUI_FRAME_HPP__
#define __GUI_FRAME_HPP__

#include "gui/visual_component.hpp"
#include "visual/writing.hpp"

#include "gui/class_export.hpp"

namespace bear
{
  namespace gui
  {
    /**
     * \brief A frame: a box with a border and a background.
     * \author Julien Jorge
     */
    class GUI_EXPORT frame:
      public visual_component
    {
    public:
      frame();
      explicit frame( const std::string& title );
      frame( const std::string& title, const visual::font& f,
             visual::size_type font_size = 0 );

      void set_title( const std::string& t );
      const std::string& get_title() const;

      void set_font( const visual::font& f );
      const visual::font& get_font() const;

      void set_font_size( visual::size_type s );
      visual::size_type get_font_size() const;

      bool close();

    protected:
      virtual bool on_close();

      visual_component& get_content();
      void fit( size_type margin = 10 );
      void display( scene_element_list& e ) const;

    private:
      void on_resized();
      visual::size_type compute_title_height() const;
      void update_displayed_title();

    private:
      /** \brief The component in this frame. */
      visual_component* m_content_frame;

      /** \brief The title of the frame. */
      std::string m_title;

      /** \brief The font used to display the title of the frame. */
      visual::font m_font;

      /** \brief The size of the font. */
      visual::size_type m_font_size;

      /** \brief The title as displayed. */
      visual::writing m_displayed_title;

    }; // class frame
  } // namespace gui
} // namespace bear

#endif // __GUI_FRAME_HPP__
