/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::reflecting_decoration class.
 * \author Sebastie Angibaud
 */
#include "generic_items/reflecting_decoration.hpp"
#include "visual/scene_element_sequence.hpp"

#include "engine/layer/layer.hpp"
#include "engine/world.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( reflecting_decoration, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two scene_visual instances on their z_position.
 * \return s1.z_position < s2.z_position
 */
bool bear::reflecting_decoration::z_item_position_compare::operator()
(  const item_handle& s1, const item_handle& s2 ) const
{
  if ( s1.get_item() == NULL )
    return false;
  else if ( s2.get_item() == NULL )
    return true;
  else
    return s1->get_z_position() < s2->get_z_position();
} // z_item_position_compare::operator()()

/*---------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 */
void bear::reflecting_decoration::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  m_items_list.clear();

  std::list<universe::physical_item*> items;

  if ( get_layer().has_world() )
    {
      bear::universe::item_picking_filter filter;
      filter.set_fixed_value(false);
      
      get_layer().get_world().pick_items_in_rectangle
        ( items, get_bounding_box(), filter);

      std::list<universe::physical_item*>::iterator it;
      for ( it = items.begin(); it != items.end(); ++it )
        {
          engine::base_item* item = dynamic_cast<engine::base_item*>(*it);

          if ( (item != NULL) && (item != this) )
            m_items_list.push_back(item_handle(*it));
        }
    }
} // reflecting_decoration::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprites representing the item.
 * \param visuals (out) The sprites of the item, and their positions.
 */
void bear::reflecting_decoration::get_visual
( std::list<engine::scene_visual>& visuals ) const
{
  items_list::const_iterator it;
  items_list item_list(m_items_list);

  item_list.sort( reflecting_decoration::z_item_position_compare() );

  for ( it = item_list.begin(); it != item_list.end(); ++it )
    if ( it->get_item() != NULL )
      {
        std::list<engine::scene_visual> scenes;
        (*it)->get_visual(scenes);

        scenes.sort(engine::scene_visual::z_position_compare());

        if ( !scenes.empty() )
          {
            visual::scene_element_sequence e;
            visual::position_type origin
              ( scenes.front().scene_element.get_position() );
            e.set_position( origin + get_gap() );

            for ( ; !scenes.empty() ; scenes.pop_front() )
              {
                visual::scene_element elem( scenes.front().scene_element );
                elem.set_position( elem.get_position() - origin );
                e.push_back( elem );
              }

            e.get_rendering_attributes().combine( get_rendering_attributes() );

            visuals.push_back( engine::scene_visual(e, get_z_position()) );
          }
      }
} // reflecting_decoration::get_visual()
