/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::link_layer class.
 * \author Julien Jorge
 */
#include "generic_items/layer/link_layer.hpp"

#include "engine/level.hpp"
#include "universe/link/base_link.hpp"
#include "visual/scene_line.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::link_layer::link_layer()
  : base_debugging_layer(bear::input::keyboard::kc_F4)
{

} // link_layer::link_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the links between the items.
 * \param e (out) The scene elements.
 * \param delta The delta to apply to the position of the items.
 */
void bear::link_layer::render
( scene_element_list& e, const bear::visual::position_type& delta ) const
{
  bear::universe::size_box_type camera_size(get_level().get_camera_size());
  claw::math::coordinate_2d<double> ratio
    ( get_size().x / camera_size.x, get_size().y / camera_size.y);
  std::list<link_type>::const_iterator it;

  for (it=m_links.begin(); it!=m_links.end(); ++it)
    {
      link_type p = *it;

      p[0].x = (p[0].x - delta.x) * ratio.x;
      p[0].y = (p[0].y - delta.y) * ratio.y;
      p[1].x = (p[1].x - delta.x) * ratio.x;
      p[1].y = (p[1].y - delta.y) * ratio.y;

      e.push_back
        ( bear::visual::scene_line(0, 0, claw::graphic::red_pixel, p, 2) );
    }
} // link_layer::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the layer.
 * \param items The items to draw.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::link_layer::progress
( const item_list& items, bear::universe::time_type elapsed_time )
{
  m_links.clear();

  item_list::const_iterator it;
  link_type points(2);
  bear::universe::physical_item::const_link_iterator it_link;

  for (it=items.begin(); it!=items.end(); ++it)
    for (it_link=(*it)->links_begin(); it_link!=(*it)->links_end(); ++it_link)
      {
        const bear::universe::physical_item& first =
          (*it_link)->get_first_item();
        const bear::universe::physical_item& second =
          (*it_link)->get_second_item();

        points[0] = first.get_center_of_mass();
        points[1] = second.get_center_of_mass();

        m_links.push_back(points);
      }
} // link_layer::progress()
