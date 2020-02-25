/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::wireframe_layer class.
 * \author Julien Jorge
 */
#include "bear/generic_items/layer/wireframe_layer.hpp"

#include "bear/engine/level.hpp"
#include "bear/visual/scene_line.hpp"

#include "bear/generic_items/slope.hpp"
#include "bear/generic_items/descending_ceiling.hpp"

#include <boost/math/special_functions/sign.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::wireframe_layer::wireframe_layer()
  : base_debugging_layer(input::keyboard::kc_F3)
{

} // wireframe_layer::wireframe_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the bounding box of the items.
 * \param e (out) The scene elements.
 * \param delta The delta to apply to the position of the items.
 */
void bear::wireframe_layer::render
( scene_element_list& e, const visual::position_type& delta ) const
{
  item_list::const_iterator it;
  visual::color_type color;

  color.components.alpha = 255;

  for (it=m_items.begin(); it!=m_items.end(); ++it)
    {
      if ( *it != NULL )
        {
          uintptr_t addr = reinterpret_cast<uintptr_t>(*it);
          
          color.components.red = (~addr & 0xFF);
          color.components.green =  (~addr & 0xFF00) >> 8;
          color.components.blue = (~addr & 0xFF0000) >> 16;
          
          draw_box(e, delta, **it, color);
          draw_internal_forces( e, delta, **it, color );
          draw_system(e, delta, **it, color);
          draw_slope(e, delta, **it, color);
        }
    }
} // wireframe_layer::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the layer.
 * \param items The items to draw.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::wireframe_layer::progress
( const item_list& items, universe::time_type elapsed_time )
{
  m_items = items;
} // wireframe_layer::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the bounding box of an item.
 * \param e (out) The scene elements.
 * \param delta The delta to apply to the position of the item.
 * \param item The item to render.
 * \param color The color of the box.
 */
void bear::wireframe_layer::draw_box
( scene_element_list& e, const visual::position_type& delta,
  const universe::physical_item& item,
  const visual::color_type& color ) const
{
  universe::size_box_type camera_size(get_level().get_camera_size());
  claw::math::coordinate_2d<double> ratio
    ( get_size().x / camera_size.x, get_size().y / camera_size.y);

  std::vector<visual::position_type> points(5);
  const visual::position_type pos( item.get_bottom_left() - delta );

  points[0] = pos;
  points[0].x = points[0].x * ratio.x;
  points[0].y = points[0].y * ratio.y;
  points[1] = points[0];
  points[1].x += item.get_width() * ratio.x;
  points[2] = points[1];
  points[2].y += item.get_height() * ratio.y;
  points[3] = points[0];
  points[3].y += item.get_height() * ratio.y;
  points[4] = points[0];

  e.push_back( visual::scene_line(0, 0, color, points, 1) );
} // wireframe_layer::draw_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the internal forces of an item.
 * \param e (out) The scene elements.
 * \param delta The delta to apply to the position of the item.
 * \param item The item to render.
 * \param color The color of the system.
 */
void bear::wireframe_layer::draw_internal_forces
( scene_element_list& e, const visual::position_type& delta,
  const universe::physical_item& item,
  const visual::color_type& color ) const
{
  universe::force_type scaled_force( item.get_internal_force() );

  if ( scaled_force.x != 0 )
    scaled_force.x =
      boost::math::sign( scaled_force.x )
      * std::log( std::abs( scaled_force.x ) );

  if ( scaled_force.y != 0 )
    scaled_force.y =
      boost::math::sign( scaled_force.y )
      * std::log( std::abs( scaled_force.y ) );

  universe::size_box_type camera_size(get_level().get_camera_size());
  claw::math::coordinate_2d<double> ratio
    ( get_size().x / camera_size.x, get_size().y / camera_size.y);

  std::vector<visual::position_type> points(3);

  universe::vector_type x_axis(item.get_x_axis());

  points[1].x = (item.get_center_of_mass().x - delta.x) * ratio.x;
  points[1].y = (item.get_center_of_mass().y - delta.y) * ratio.y;
  points[0] = points[1] + scaled_force.x * x_axis;
  points[2] =
    points[1] + scaled_force.y * x_axis.get_orthonormal_anticlockwise();

  e.push_back( visual::scene_line(0, 0, color, points, 3) );
} // wireframe_layer::draw_internal_forces()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the local system of an item.
 * \param e (out) The scene elements.
 * \param delta The delta to apply to the position of the item.
 * \param item The item to render.
 * \param color The color of the system.
 */
void bear::wireframe_layer::draw_system
( scene_element_list& e, const visual::position_type& delta,
  const universe::physical_item& item,
  const visual::color_type& color ) const
{
  universe::size_box_type camera_size(get_level().get_camera_size());
  claw::math::coordinate_2d<double> ratio
    ( get_size().x / camera_size.x, get_size().y / camera_size.y);

  std::vector<visual::position_type> points(3);

  universe::vector_type x_axis(item.get_x_axis());

  points[1].x = (item.get_center_of_mass().x - delta.x) * ratio.x;
  points[1].y = (item.get_center_of_mass().y - delta.y) * ratio.y;
  points[0] = points[1] + 20 * x_axis;
  points[2] = points[1] + 20 * x_axis.get_orthonormal_anticlockwise();

  e.push_back( visual::scene_line(0, 0, color, points, 1) );
} // wireframe_layer::draw_system()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the lines of the slopes.
 * \param e (out) The scene elements.
 * \param delta The delta to apply to the position of the item.
 * \param item The item to render.
 * \param color The color of the system.
 */
void bear::wireframe_layer::draw_slope
( scene_element_list& e, const visual::position_type& delta,
  const universe::physical_item& item,
  const visual::color_type& color ) const
{
  universe::size_box_type camera_size(get_level().get_camera_size());
  claw::math::coordinate_2d<double> ratio
    ( get_size().x / camera_size.x, get_size().y / camera_size.y);

  const slope* p = dynamic_cast<const slope*>(&item);

  if ( p != NULL )
    {
      std::vector<visual::position_type> pts;

      const slope::curve_type c( p->get_curve() );

      for ( double t=0; t<=1; t+=0.1 )
        {
          pts.push_back
            ( p->get_bottom_left() + c.get_section(c.begin()).get_point_at(t) );

          const visual::coordinate_type x =
            c.get_section(c.begin()).get_point_at(t).x;
                        
          if ( c.get_section(c.begin()).get_point_at_x( x ).size() > 0 )
            pts.back().y =
              ( p->get_bottom() + 
                c.get_section
                (c.begin()).get_point_at_x( x )[0].get_position().y);
          
          pts.back().x -= delta.x;
          pts.back().y -= delta.y;
              
          pts.back().x *= ratio.x;
          pts.back().y *= ratio.y;
        }

      e.push_back( visual::scene_line(0, 0, color, pts, 1) );
    }

  const descending_ceiling* d =
    dynamic_cast<const descending_ceiling*>(&item);

  if ( d != NULL )
    {
      std::vector<visual::position_type> pts(2);
      pts[0].x = (item.get_left() - delta.x) * ratio.x;
      pts[0].y = (item.get_bottom() - delta.y) * ratio.y;

      if ( d->get_steepness() < 0 )
        pts[0].y =
          (item.get_bottom() - d->get_steepness() - delta.y) * ratio.y;
      else
        pts[0].y = (item.get_bottom() - delta.y) * ratio.y;

      pts[1].x = (item.get_right() - delta.x) * ratio.x;
      pts[1].y = pts[0].y + d->get_steepness() * ratio.y;

      e.push_back( visual::scene_line(0, 0, color, pts, 1) );
    }
} // wireframe_layer::draw_slope()
