/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::recent_path_layer class.
 * \author Julien Jorge
 */
#include <algorithm>

#include "generic_items/layer/recent_path_layer.hpp"
#include "input/keyboard.hpp"
#include "visual/scene_line.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::recent_path_layer::item_positions::item_positions
( const bear::universe::physical_item* item )
  : super(9)
{
  this->operator[](0) = item->get_top_left();
  this->operator[](1) = item->get_top_middle();
  this->operator[](2) = item->get_top_right();
  this->operator[](3) = item->get_left_middle();
  this->operator[](4) = item->get_center_of_mass();
  this->operator[](5) = item->get_right_middle();
  this->operator[](6) = item->get_bottom_left();
  this->operator[](7) = item->get_bottom_middle();
  this->operator[](8) = item->get_bottom_right();
} // recent_path_layer::item_positions::item_positions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the position of an item has changed.
 * \param item The item from which we tak the position.
 */
bool bear::recent_path_layer::item_positions::has_moved
( const bear::universe::physical_item* item ) const
{
  return this->operator[](0) != item->get_top_left();
} // recent_path_layer::item_positions::has_moved()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::recent_path_layer::recent_path_layer()
  : base_debugging_layer(bear::input::keyboard::kc_F7)
{

} // recent_path_layer::recent_path_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the lines of the items.
 * \param e (out) The scene elements.
 * \param delta The delta to apply to the position of the items.
 */
void bear::recent_path_layer::render
( scene_element_list& e, const bear::visual::position_type& delta ) const
{
  item_map::const_iterator itm;

  // one color for each line.
  bear::visual::color_type colors[9] =
    {
      claw::graphic::white_pixel,
      claw::graphic::red_pixel,
      claw::graphic::green_pixel,
      claw::graphic::blue_pixel,
      claw::graphic::yellow_pixel,
      claw::graphic::magenta_pixel,
      claw::graphic::cyan_pixel,
      bear::visual::color_type( 190, 190, 190, 255 ),
      bear::visual::color_type( 190, 190,   0, 255 )
    };

  std::vector<bear::visual::position_type> cross_v(2);
  std::vector<bear::visual::position_type> cross_h(2);

  cross_v[0] = bear::visual::position_type(0, -2);
  cross_v[1] = bear::visual::position_type(0, 2);

  cross_h[0] = bear::visual::position_type(-2, 0);
  cross_h[1] = bear::visual::position_type(2, 0);

  // Draw the lines.
  for (itm=m_items.begin(); itm!=m_items.end(); ++itm)
    for (unsigned int i=0; i!=9; ++i)
      {
        std::vector<bear::visual::position_type> points;
        std::list<item_positions>::const_iterator itl;
        points.reserve( itm->second.size() );

        for ( itl=itm->second.begin(); itl!=itm->second.end(); ++itl )
          {
            const bear::universe::position_type pos
              ((*itl)[i].x - delta.x, (*itl)[i].y - delta.y);

            points.push_back(pos);

            std::vector<bear::visual::position_type> cross(2);

            cross[0] = pos + cross_v[0];
            cross[1] = pos + cross_v[1];
            e.push_back
              ( bear::visual::scene_line(0, 0, colors[i], cross, 1) );

            cross[0] = pos + cross_h[0];
            cross[1] = pos + cross_h[1];
            e.push_back
              ( bear::visual::scene_line(0, 0, colors[i], cross, 1) );
          }

        e.push_back( bear::visual::scene_line(0, 0, colors[i], points, 1) );
      }
} // recent_path_layer::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the layer.
 * \param items The items to draw.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::recent_path_layer::progress
( const item_list& items, bear::universe::time_type elapsed_time )
{
  item_map::iterator itm;

  // remove lost items.
  for ( itm=m_items.begin(); itm!=m_items.end(); )
    if ( std::find(items.begin(), items.end(), itm->first) == items.end() )
      {
        item_map::iterator tmp(itm);
        ++itm;
        m_items.erase(tmp);
      }
    else
      ++itm;

  // add new items and update the positions of the known items.
  item_list::const_iterator it;
  for (it=items.begin(); it!=items.end(); ++it)
    {
      itm = m_items.find(*it);

      if ( itm == m_items.end() )
        m_items[*it].push_front( item_positions(*it) );
      else if ( itm->second.back().has_moved(*it) )
        {
          if ( itm->second.size() == 10 )
            itm->second.pop_front();

          itm->second.push_back( item_positions(*it) );
        }
    }
} // recent_path_layer::progress()
