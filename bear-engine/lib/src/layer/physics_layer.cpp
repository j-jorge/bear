/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::physics_layer class.
 * \author Julien Jorge
 */
#include "bear/generic_items/layer/physics_layer.hpp"

#include "bear/engine/level.hpp"
#include "bear/visual/scene_rectangle.hpp"

#include "bear/generic_items/camera.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::physics_layer::physics_layer()
  : base_debugging_layer(bear::input::keyboard::kc_F6)
{

} // physics_layer::physics_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the bounding box of the items.
 * \param e (out) The scene elements.
 * \param delta The delta to apply to the position of the items.
 */
void bear::physics_layer::render
( scene_element_list& e, const bear::visual::position_type& delta ) const
{
  item_list::const_iterator it;

  for (it=m_items.begin(); it!=m_items.end(); ++it)
    draw_box(e, delta, **it);
} // physics_layer::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the layer.
 * \param items The items to draw.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::physics_layer::progress
( const item_list& items, bear::universe::time_type elapsed_time )
{
  m_items = items;
} // physics_layer::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the bounding box of an item.
 * \param e (out) The scene elements.
 * \param delta The delta to apply to the position of the item.
 * \param item The item to render.
 */
void bear::physics_layer::draw_box
( scene_element_list& e, const bear::visual::position_type& delta,
  const bear::universe::physical_item& item ) const
{
  if ( dynamic_cast<const bear::camera*>(&item) != NULL )
    return;

  bear::visual::color_type border_color = claw::graphic::black_pixel;
  bear::visual::color_type fill_color(border_color);

  border_color.components.alpha = 196;
  fill_color.components.alpha = 196;

  if ( item.can_move_items() )
    fill_color.components.red = 255;

  if ( item.is_phantom() )
    fill_color.components.green = 255;

  if ( item.is_artificial() )
    fill_color.components.blue = 255;

  if ( item.is_global() )
    border_color.components.red = 255;

  if ( item.has_weak_collisions() )
    border_color.components.green = 255;

  bear::universe::size_box_type camera_size(get_level().get_camera_size());
  claw::math::coordinate_2d<double> ratio
    ( get_size().x / camera_size.x, get_size().y / camera_size.y);

  const bear::visual::position_type pos( item.get_bottom_left() - delta );

  bear::visual::rectangle_type r
    ( pos.x, pos.y, pos.x + item.get_width(), pos.y + item.get_height() );

  if ( (r.width() > 5) && (r.height() > 5) )
    r.set( r.left() + 5, r.bottom() + 5, r.right() - 5, r.top() - 5 );

  r.set
    ( r.left() * ratio.x, r.bottom() * ratio.y, r.right() * ratio.x,
      r.top() * ratio.y );

  e.push_back
    ( bear::visual::scene_rectangle( 0, 0, fill_color, r, true ) );
  e.push_back
    ( bear::visual::scene_rectangle( 0, 0, border_color, r, false, 3 ) );
} // physics_layer::draw_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the subclasses that the layer is displayed.
 */
void bear::physics_layer::on_show()
{
  std::cout << "with c=can_move_items, p=phantom, a=artificial.\n"
            << "fill color:\n"
            << "\tblack:   not c and not p and not a.\n"
            << "\tgreen:   not c and     p and not a.\n"
            << "\tyellow:      c and     p and not a.\n"
            << "\tred:         c and not p and not a.\n"
            << "\tblue:    not c and not p and     a.\n"
            << "\tcyan:    not c and     p and     a.\n"
            << "\twhite:       c and     p and     a.\n"
            << "\tmagenta:     c and not p and     a.\n"
            << '\n'
            << "with g=global, w=weak_collisions.\n"
            << "border color:\n"
            << "\tblack: not g and not w.\n"
            << "\tred:        g and not w.\n"
            << "\tgreen:  not g and     w.\n"
            << "\tyellow:     g and     w.\n"
            << std::endl;
} // physics_layer::on_show()
