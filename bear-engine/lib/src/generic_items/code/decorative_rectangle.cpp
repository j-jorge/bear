/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::decorative_rectangle class.
 * \author Sebastie Angibaud
 */
#include "generic_items/decorative_rectangle.hpp"
#include "engine/export.hpp"

#include "visual/scene_rectangle.hpp"
#include "visual/scene_polygon.hpp"

BASE_ITEM_EXPORT( decorative_rectangle, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::decorative_rectangle::decorative_rectangle()
: m_border_size(1)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
  m_fill_color.set(1,1,1,0);
  m_border_color.set(1,1,1,0);
} // decorative_rectangle::decorative_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprites representing the item.
 * \param visuals (out) The sprites of the item, and their positions.
 */
void bear::decorative_rectangle::get_visual
( std::list<engine::scene_visual>& visuals ) const
{
  super::get_visual(visuals);

  if ( get_rendering_attributes().get_opacity() != 0 )
    {
      std::vector<bear::universe::position_type> points;
      points.push_back(bear::universe::position_type(0, 0));
      points.push_back(bear::universe::position_type(get_width(), 0));
      points.push_back(bear::universe::position_type
                       (get_width(), get_height()));
      points.push_back(bear::universe::position_type(0, get_height()));
      
      bear::universe::rectangle_type rect(points[0], points[2]);
      
      if ( m_fill_color.get_opacity() != 0 )
        {
          bear::visual::scene_element e2
            ( bear::visual::scene_polygon
              (  get_left(), get_bottom(), m_fill_color, points ) );
          e2.get_rendering_attributes().combine(get_rendering_attributes());
          visuals.push_front(e2);
        }
      
      if ( m_border_color.get_opacity() != 0 )
        {
          bear::visual::scene_element e1
            (bear::visual::scene_rectangle
             ( get_left(), get_bottom(), m_border_color, rect, false, 
               m_border_size));
          e1.get_rendering_attributes().combine(get_rendering_attributes());
          visuals.push_front(e1);
        }
    }
} // decorative_rectangle::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::decorative_rectangle::set_real_field
( const std::string& name, double value )
{
  bool ok = true;

  if (name == "decorative_rectangle.border_size")
    m_border_size = value;
  else
    ok = super::set_real_field(name, value);

  return ok;
} // decorative_rectangle::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c color.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::decorative_rectangle::set_color_field
( const std::string& name, visual::color value )
{
  bool ok = true;
  
  if (name == "decorative_rectangle.fill_color")
    m_fill_color = value;
  else if (name == "decorative_rectangle.border_color")
    m_border_color = value;
  else
    ok = super::set_color_field(name, value);

  return ok;
} // decorative_rectangle::set_color_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the fill color.
 * \param c The fill color.
 */
void bear::decorative_rectangle::set_fill_color( const visual::color& c )
{
  m_fill_color = c;
} // decorative_rectangle::set_fill_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the border color.
 * \param c The border color.
 */
void bear::decorative_rectangle::set_border_color( const visual::color& c )
{
  m_border_color = c;
} // decorative_rectangle::set_border_color()
