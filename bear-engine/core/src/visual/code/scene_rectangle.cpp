/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the visual::scene_rectangle.
 * \author Julien Jorge
 */
#include "visual/scene_rectangle.hpp"

#include "visual/base_screen.hpp"
#include "visual/scene_element.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param x X-position of the origin of the rectangle on the screen.
 * \param y Y-position of the origin of the rectangle on the screen.
 * \param color The color of the rectangle.
 * \param r The rectangle.
 * \param fill Tell if the rectangle is filled or not.
 * \param border_weight The width of the border for a not filled rectangle.
 */
bear::visual::scene_rectangle::scene_rectangle
( coordinate_type x, coordinate_type y, const color_type& color,
  const rectangle_type& r, bool fill, size_type border_weight )
  : base_scene_element(x, y), m_color(color), m_rectangle(r), m_fill(fill),
    m_border_weight(border_weight)
{

} // scene_rectangle::scene_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
bear::visual::base_scene_element* bear::visual::scene_rectangle::clone() const
{
  return new scene_rectangle(*this);
} // scene_rectangle::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a rectangle where the rectangle is completely opaque.
 */
bear::visual::rectangle_type
bear::visual::scene_rectangle::get_opaque_box() const
{
  rectangle_type result(0, 0, 0, 0);

  if ( m_fill && (get_rendering_attributes().get_opacity() == 1)
       && ( m_color.components.alpha
            == std::numeric_limits<color_type::component_type>::max() ) )
    result = get_bounding_box();

  return result;
} // scene_rectangle::get_opaque_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a rectangle bounding the rectangle.
 */
bear::visual::rectangle_type
bear::visual::scene_rectangle::get_bounding_box() const
{
  position_type p( get_position() + m_rectangle.bottom_left() );

  return rectangle_type
    ( p.x, p.y,
      p.x + m_rectangle.width() * get_scale_factor_x(),
      p.y + m_rectangle.height() * get_scale_factor_y() );
} // scene_rectangle::get_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Split the rectangle in sub rectangles.
 * \param boxes The boxes describing how to split the rectangle.
 * \param output The resulting rectangles (they are inserted to the back).
 */
void bear::visual::scene_rectangle::burst
( const rectangle_list& boxes, scene_element_list& output ) const
{
  if ( !m_fill )
    {
      output.push_back( *this );
      return;
    }

  rectangle_type my_rect(get_bounding_box());

  for (rectangle_list::const_iterator it=boxes.begin(); it!=boxes.end(); ++it)
    if ( my_rect.intersects(*it) )
      {
        rectangle_type r( my_rect.intersection(*it) );

        if ( !r.empty() )
          {
            scene_rectangle n(0, 0, m_color, r, m_fill);
            n.set_rendering_attributes(get_rendering_attributes());
            n.set_scale_factor(1, 1);
            output.push_back( n );
          }
      }
} // scene_rectangle::burst()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the rectangle on a screen.
 * \param scr The screen on which we render the rectangle.
 */
void bear::visual::scene_rectangle::render( base_screen& scr ) const
{
  rectangle_type my_rect(get_bounding_box());

  std::vector<position_type> p(4);

  p[0] = my_rect.bottom_left();
  p[1] = my_rect.top_left();
  p[2] = my_rect.top_right();
  p[3] = my_rect.bottom_right();

  color_type c( m_color );

  c.components.red *= get_rendering_attributes().get_red_intensity();
  c.components.green *= get_rendering_attributes().get_green_intensity();
  c.components.blue *= get_rendering_attributes().get_blue_intensity();
  c.components.alpha *= get_rendering_attributes().get_opacity();

  if (m_fill)
    scr.draw_polygon(c, p);
  else
    {
      p.push_back(p[0]);
      scr.draw_line(c, p, m_border_weight);
    }
} // scene_rectangle::render()
