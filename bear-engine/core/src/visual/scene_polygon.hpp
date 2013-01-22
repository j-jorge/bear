/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A filled polygon on the screen.
 * \author Julien Jorge
 */
#ifndef __VISUAL_SCENE_POLYGON_HPP__
#define __VISUAL_SCENE_POLYGON_HPP__

#include "visual/base_scene_element.hpp"

#include "visual/class_export.hpp"
#include <vector>

namespace bear
{
  namespace visual
  {
    /**
     * \brief A filled polygon on the screen.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT scene_polygon:
      public base_scene_element
    {
    public:
      scene_polygon
      ( coordinate_type x, coordinate_type y,
        const color_type& color,
        const std::vector<position_type>& p );

      base_scene_element* clone() const;

      rectangle_type get_opaque_box() const;
      rectangle_type get_bounding_box() const;

      void burst
      ( const rectangle_list& boxes, scene_element_list& output ) const;

      void render( base_screen& scr ) const;

    private:
      /** \brief The color of the line. */
      const color_type m_color;

      /** \brief The vertices of the line. */
      const std::vector<position_type> m_points;

    }; // class scene_polygon
  } // namespace visual
} // namespace bear

#endif // __VISUAL_SCENE_POLYGON_HPP__
