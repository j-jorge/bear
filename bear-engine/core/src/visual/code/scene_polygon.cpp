/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the visual::scene_polygon.
 * \author Julien Jorge
 */
#include "visual/scene_polygon.hpp"

#include "visual/base_screen.hpp"
#include "visual/scene_element.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param x X-position of the origin of the polygon on the screen.
 * \param y Y-position of the origin of the polygon on the screen.
 * \param color The color of the polygon.
 * \param p The points of the polygon.
 */
bear::visual::scene_polygon::scene_polygon
( coordinate_type x, coordinate_type y, const color_type& color,
  const std::vector<position_type>& p )
  : base_scene_element(x, y), m_color(color), m_points(p)
{

} // scene_polygon::scene_polygon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
bear::visual::base_scene_element* bear::visual::scene_polygon::clone() const
{
  return new scene_polygon(*this);
} // scene_polygon::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a rectangle where the polygon is completely opaque.
 */
bear::visual::rectangle_type bear::visual::scene_polygon::get_opaque_box() const
{
  return rectangle_type(0, 0, 0, 0);
} // scene_polygon::get_opaque_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a rectangle bounding the polygon.
 */
bear::visual::rectangle_type
bear::visual::scene_polygon::get_bounding_box() const
{
  if ( m_points.empty() )
    return rectangle_type(0, 0, 0, 0);

  coordinate_type min_x = m_points[0].x;
  coordinate_type min_y = m_points[0].y;
  coordinate_type max_x = min_x;
  coordinate_type max_y = min_y;

  for (unsigned int i=0; i!=m_points.size(); ++i)
    {
      if ( m_points[i].x < min_x )
        min_x = m_points[i].x;

      if ( m_points[i].y < min_y )
        min_y = m_points[i].y;

      if ( m_points[i].x > max_x )
        max_x = m_points[i].x;

      if ( m_points[i].y > max_y )
        max_y = m_points[i].y;
    }

  min_x = get_position().x + min_x * get_scale_factor_x();
  min_y = get_position().y + min_y * get_scale_factor_y();
  max_x = get_position().x + max_x * get_scale_factor_x();
  max_y = get_position().y + max_y * get_scale_factor_y();

  return rectangle_type(min_x, min_y, max_x, max_y);
} // scene_polygon::get_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Split the polygon in sub polygons.
 * \param boxes The boxes describing how to split the polygon.
 * \param output The resulting polygons (they are inserted to the front).
 */
void bear::visual::scene_polygon::burst
( const rectangle_list& boxes, scene_element_list& output ) const
{
  output.push_back( scene_element(*this) );
} // scene_polygon::burst()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the polygon on a screen.
 * \param scr The screen on which we render the polygon.
 */
void bear::visual::scene_polygon::render( base_screen& scr ) const
{
  std::vector<position_type> p(m_points);

  for ( std::size_t i=0; i!=p.size(); ++i )
    {
      p[i].x = get_position().x + p[i].x * get_scale_factor_x();
      p[i].y = get_position().y + p[i].y * get_scale_factor_y();
    }

  color_type c( m_color );

  c.components.red *= get_rendering_attributes().get_red_intensity();
  c.components.green *= get_rendering_attributes().get_green_intensity();
  c.components.blue *= get_rendering_attributes().get_blue_intensity();
  c.components.alpha *= get_rendering_attributes().get_opacity();

  scr.draw_polygon(c, p);
} // scene_polygon::render()
