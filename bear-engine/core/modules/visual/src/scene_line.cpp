/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the visual::scene_line.
 * \author Julien Jorge
 */
#include "bear/visual/scene_line.hpp"

#include "bear/visual/base_screen.hpp"
#include "bear/visual/scene_element.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param x X-position of the origin of the line on the screen.
 * \param y Y-position of the origin of the line on the screen.
 * \param color The color of the line.
 * \param p The points of the line.
 * \param w The width of the line.
 */
bear::visual::scene_line::scene_line
( coordinate_type x, coordinate_type y, const color_type& color,
  const std::vector<position_type>& p, double w )
  : base_scene_element(x, y), m_color(color), m_points(p), m_width(w)
{

} // scene_line::scene_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
bear::visual::base_scene_element* bear::visual::scene_line::clone() const
{
  return new scene_line(*this);
} // scene_line::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a rectangle where the line is completely opaque.
 */
bear::visual::rectangle_type bear::visual::scene_line::get_opaque_box() const
{
  return rectangle_type(0, 0, 0, 0);
} // scene_line::get_opaque_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a rectangle bounding the line.
 */
bear::visual::rectangle_type
bear::visual::scene_line::get_bounding_box() const
{
  if ( m_points.empty() )
    return rectangle_type(0, 0, 0, 0);

  coordinate_type min_x = m_points[0].x - m_width / 2;
  coordinate_type min_y = m_points[0].y - m_width / 2;
  coordinate_type max_x = m_points[0].x + m_width / 2;
  coordinate_type max_y = m_points[0].y + m_width / 2;

  for (unsigned int i=0; i!=m_points.size(); ++i)
    {
      if ( m_points[i].x - m_width / 2 < min_x )
        min_x = m_points[i].x - m_width / 2;

      if ( m_points[i].y - m_width / 2 < min_y )
        min_y = m_points[i].y - m_width / 2;

      if ( m_points[i].x + m_width / 2 > max_x )
        max_x = m_points[i].x + m_width / 2;

      if ( m_points[i].y + m_width / 2 > max_y )
        max_y = m_points[i].y + m_width / 2;
    }

  min_x = get_position().x + min_x * get_scale_factor_x();
  min_y = get_position().y + min_y * get_scale_factor_y();
  max_x = get_position().x + max_x * get_scale_factor_x();
  max_y = get_position().y + max_y * get_scale_factor_y();

  return rectangle_type(min_x, min_y, max_x, max_y);
} // scene_line::get_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Split the line in sub lines.
 * \param boxes The boxes describing how to split the line.
 * \param output The resulting lines (they are inserted to the back).
 */
void bear::visual::scene_line::burst
( const rectangle_list& boxes, scene_element_list& output ) const
{
  output.push_back( scene_element(*this) );
} // scene_line::burst()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the line on a screen.
 * \param scr The screen on which we render the line.
 */
void bear::visual::scene_line::render( base_screen& scr ) const
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

  scr.draw_line(c, p, m_width);
} // scene_line::render()
