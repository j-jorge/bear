/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A simple component to display any visual::scene_element
 *        in windows or any visual component.
 * \author Julien Jorge
 */
#ifndef __GUI_SCENE_ELEMENT_HPP__
#define __GUI_SCENE_ELEMENT_HPP__

#include "gui/visual_component.hpp"
#include "visual/scene_element.hpp"

#include "gui/class_export.hpp"

namespace bear
{
  namespace gui
  {
    /**
     * \brief A simple component to display any visual::scene_element
     *        in windows or any visual component.
     * \author Julien Jorge
     */
    class GUI_EXPORT scene_element:
      public visual_component
    {
    public:
      explicit scene_element
      ( const visual::scene_element& e = visual::scene_element() );

      void set_scene_element( const visual::scene_element& e );

    private:
      void display( scene_element_list& e ) const;

      virtual void on_resized();

      void stretch_element();

    private:
      /** \brief The element to render. */
      visual::scene_element m_element;

      /** \brief Tell if the element is stretched to fill the control. */
      bool m_stretch;

    }; // class scene_element
  } // namespace gui
} // namespace bear

#endif // __GUI_SCENE_ELEMENT_HPP__
