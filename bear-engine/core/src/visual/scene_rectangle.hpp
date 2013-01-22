/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A filled rectangle on the screen.
 * \author Julien Jorge
 */
#ifndef __VISUAL_SCENE_RECTANGLE_HPP__
#define __VISUAL_SCENE_RECTANGLE_HPP__

#include "visual/base_scene_element.hpp"

#include "visual/class_export.hpp"
#include <vector>

namespace bear
{
  namespace visual
  {
    /**
     * \brief A rectangle on the screen, filled or not.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT scene_rectangle:
      public base_scene_element
    {
    public:
      scene_rectangle
      ( coordinate_type x, coordinate_type y,
        const color_type& color, const rectangle_type& r, bool fill = true,
        size_type border_weight = 1 );

      base_scene_element* clone() const;

      rectangle_type get_opaque_box() const;
      rectangle_type get_bounding_box() const;

      void burst
      ( const rectangle_list& boxes, scene_element_list& output ) const;

      void render( base_screen& scr ) const;

    private:
      /** \brief The color of the line. */
      const color_type m_color;

      /** \brief The rectangle to draw. */
      const rectangle_type m_rectangle;

      /** \brief Tell if the rectangle is filled or not. */
      const bool m_fill;

      /** \brief The width of the border for a not filled rectangle. */
      const size_type m_border_weight;

    }; // class scene_rectangle
  } // namespace visual
} // namespace bear

#endif // __VISUAL_SCENE_RECTANGLE_HPP__
