/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::star class.
 * \author Julien Jorge
 */
#include "bear/generic_items/star.hpp"

#include "bear/visual/scene_star.hpp"

#include <limits>

BASE_ITEM_EXPORT(star, bear)

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::star::star()
: m_star(5, 0.5), m_fill_color(claw::graphic::black_pixel),
  m_border_color(claw::graphic::black_pixel), m_border_width(1)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // star::star()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param branches The number of branches.
 * \param ratio The ratio of the inner vertices relatively to the outer ones.
 * \param border_color The red component of the color of the border.
 * \param border_width The width of the border of the star.
 * \param fill_color The red component of the color of the inside.
 */
bear::star::star
( std::size_t branches, double ratio, const visual::color_type& border_color,
  double border_width, const visual::color_type& fill_color )
  : m_star(branches, ratio), m_fill_color(fill_color),
    m_border_color(border_color), m_border_width(border_width)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // star::star()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do post creation actions.
 */
void bear::star::on_enters_layer()
{
  super::on_enters_layer();

  get_rendering_attributes().set_size( get_size() );
} // star::on_enters_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type unsigned int.
 * \param name The name of the field to set.
 * \param value The value of the field.
 */
bool
bear::star::set_u_integer_field( const std::string& name, unsigned int value )
{
  bool result(true);

  if ( name == "star.branches" )
    m_star.set_branches(value);
  else
    result = super::set_u_integer_field(name, value);

  return result;
} // star::set_u_integer_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type real.
 * \param name The name of the field to set.
 * \param value The value of the field.
 */
bool bear::star::set_real_field( const std::string& name, double value )
{
  bool result(true);

  const visual::color_type::component_type c =
    std::numeric_limits<visual::color_type::component_type>::max();

  if ( name == "star.ratio" )
    m_star.set_ratio(value);
  else if ( name == "star.border_width" )
    m_border_width = value;
  else if ( name == "star.border_color.red" )
    m_border_color.components.red = value * c;
  else if ( name == "star.border_color.green" )
    m_border_color.components.green = value * c;
  else if ( name == "star.border_color.blue" )
    m_border_color.components.blue = value * c;
  else if ( name == "star.border_color.opacity" )
    m_border_color.components.alpha = value * c;
  else if ( name == "star.fill_color.red" )
    m_fill_color.components.red = value * c;
  else if ( name == "star.fill_color.green" )
    m_fill_color.components.green = value * c;
  else if ( name == "star.fill_color.blue" )
    m_fill_color.components.blue = value * c;
  else if ( name == "star.fill_color.opacity" )
    m_fill_color.components.alpha = value * c;
  else
    result = super::set_real_field(name, value);

  return result;
} // star::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visual of the item.
 * \param visual (out) The visual representation of the item.
 */
void bear::star::get_visual( std::list<engine::scene_visual>& visuals ) const
{
  super::get_visual(visuals);

  visual::scene_star s
    ( 0, 0, m_border_color, m_star, m_border_width, m_fill_color );
  s.get_rendering_attributes().set_size
    ( get_rendering_attributes().get_size() );

  engine::scene_visual v = get_scene_visual(s);
  v.scene_element.set_position
    ( get_horizontal_middle() - v.scene_element.get_width() / 2,
      get_vertical_middle() - v.scene_element.get_height() / 2 );

  visuals.push_back(v);
} // star::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the ratio of the distance to the center of the inner vertices
 *        relatively to the distance of the outer ones.
 * \param r The ratio.
 */
void bear::star::set_ratio( double r )
{
  m_star.set_ratio(r);
} // star::set_ratio()
