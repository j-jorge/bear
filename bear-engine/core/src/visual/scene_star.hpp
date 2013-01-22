/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A star on the screen.
 * \author Julien Jorge
 */
#ifndef __VISUAL_SCENE_STAR_HPP__
#define __VISUAL_SCENE_STAR_HPP__

#include "visual/base_scene_element.hpp"
#include "visual/star.hpp"

#include "visual/class_export.hpp"
#include <vector>

namespace bear
{
  namespace visual
  {
    /**
     * \brief A star on the screen, filled or not.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT scene_star:
      public base_scene_element
    {
    public:
      scene_star
      ( coordinate_type x, coordinate_type y, const color_type& color,
        const star& s, double border_width = 1,
        const color_type& fill_color = claw::graphic::transparent_pixel );

      base_scene_element* clone() const;

      rectangle_type get_opaque_box() const;
      void render( base_screen& scr ) const;

      position_type get_center() const;

    private:
      void render_inside
        ( base_screen& scr,
          const std::vector<position_type>& coordinates ) const;
      void render_border
        ( base_screen& scr,
          const std::vector<position_type>& coordinates ) const;

      void compute_coordinates( std::vector<position_type>& coordinates ) const;

    private:
      /** \brief The color of the line. */
      const color_type m_color;

      /** \brief The width of the border. */
      const double m_border_width;

      /** \brief The color of the inside of the star. */
      const color_type m_fill_color;

      /** \brief The star to display. */
      const star m_star;

    }; // class scene_star
  } // namespace visual
} // namespace bear

#endif // __VISUAL_SCENE_STAR_HPP__
