/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::scene_star class.
 * \author Julien Jorge
 */
#include "visual/scene_star.hpp"

#include "visual/base_screen.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param x X-position of the element on the screen.
 * \param y Y-position of the element on the screen.
 * \param color The color of the border of the star.
 * \param s The star to display.
 * \param border_width The width of the border of the star.
 * \param fill_color The color of the inside of the star.
 */
bear::visual::scene_star::scene_star
( coordinate_type x, coordinate_type y, const color_type& color,
  const star& s, double border_width, const color_type& fill_color )
  : base_scene_element(x, y), m_color(color), m_border_width(border_width),
    m_fill_color(fill_color), m_star(s)
{

} // scene_star::scene_star()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this element.
 */
bear::visual::base_scene_element* bear::visual::scene_star::clone() const
{
  return new scene_star(*this);
} // scene_star::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a box where the element is fully opaque.
 */
bear::visual::rectangle_type bear::visual::scene_star::get_opaque_box() const
{
  if ( (m_fill_color.components.alpha !=
        std::numeric_limits<color_type::component_type>::max())
       || ( get_rendering_attributes().get_opacity() != 1 ) )
    return rectangle_type(0, 0, 0, 0);
  else
    {
      const position_type c = get_center();
      const size_type aw = m_star.get_ratio() * std::cos(3.14159/4);
      const size_type ah = m_star.get_ratio() * std::sin(3.14159/4);
      const size_type w =
        get_rendering_attributes().width() * aw / 2 * get_scale_factor_x();
      const size_type h =
        get_rendering_attributes().height() * ah / 2 * get_scale_factor_y();

      return rectangle_type( c.x - w, c.y - h, c.x + w, c.y + h );
    }
} // base_scene_element::get_opaque_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the element on the screen.
 * \param scr The screen on which the star is rendered.
 */
void bear::visual::scene_star::render( base_screen& scr ) const
{
  std::vector<position_type> coordinates;

  compute_coordinates( coordinates );

  render_inside( scr, coordinates );
  render_border( scr, coordinates );
} // scene_star::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the center of the star.
 */
bear::visual::position_type bear::visual::scene_star::get_center() const
{
  const size_type w = get_rendering_attributes().width() * get_scale_factor_x();
  const size_type h =
    get_rendering_attributes().height() * get_scale_factor_y();

  return  get_position() + position_type(w, h) / 2;
} // scene_star::get_center()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the inside of the star on the screen.
 * \param scr The screen on which the star is rendered.
 * \param coordinates The coordinates of the vertices of the star.
 */
void bear::visual::scene_star::render_inside
( base_screen& scr, const std::vector<position_type>& coordinates ) const
{
  if ( m_fill_color.components.alpha == 0 )
    return;

  const color_type c = get_rendering_attributes().convert_color(m_fill_color);

  std::vector<position_type> branch(4);
  branch[0] = get_center();
  branch[1] = coordinates[1];
  branch[2] = coordinates[0];
  branch[3] = coordinates.back();
  scr.draw_polygon( c, branch );

  for ( std::size_t i=2; i<coordinates.size()-1; i+=2 ) {
    branch[1] = coordinates[i+1];
    branch[2] = coordinates[i];
    branch[3] = coordinates[i-1];
    scr.draw_polygon( c, branch );
  }
} // scene_star::render_inside()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the outside of the star on the screen.
 * \param scr The screen on which the star is rendered.
 * \param coordinates The coordinates of the vertices of the star.
 */
void bear::visual::scene_star::render_border
( base_screen& scr, const std::vector<position_type>& coordinates ) const
{
  scr.draw_line
    ( get_rendering_attributes().convert_color(m_color), coordinates,
      m_border_width, true );
} // scene_star::render_border()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the coordinates of the vertices.
 * \param coordinates (out) The coordinates.
 */
void bear::visual::scene_star::compute_coordinates
( std::vector<position_type>& coordinates ) const
{
  const position_type c = get_center();
  const size_type w =
    (get_rendering_attributes().width() * get_scale_factor_x()) / 2;
  const size_type h =
    (get_rendering_attributes().height() * get_scale_factor_y()) / 2;
  const double a = get_rendering_attributes().get_angle();
  const double dir_x = get_rendering_attributes().is_mirrored() ? -1 : 1;
  const double dir_y = get_rendering_attributes().is_flipped() ? -1 : 1;

  coordinates = m_star.get_coordinates();

  for ( std::size_t i=0; i!=coordinates.size(); ++i )
    {
      const coordinate_type x = coordinates[i].x * dir_x;
      const coordinate_type y = coordinates[i].y * dir_y;

      coordinates[i].x = c.x + (x * std::cos(a) - y * std::sin(a)) * w;
      coordinates[i].y = c.y + (x * std::sin(a) + y * std::cos(a)) * h;
    }
} // scene_star::compute_coordinates()
