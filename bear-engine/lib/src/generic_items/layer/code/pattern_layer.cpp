/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::pattern_layer class.
 * \author Julien Jorge
 */
#include <algorithm>

#include "generic_items/layer/pattern_layer.hpp"

#include "engine/layer/export.hpp"

#include <claw/assert.hpp>

LAYER_EXPORT( pattern_layer, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param size The size of the layer.
 */
bear::pattern_layer::pattern_layer
( const universe::size_box_type& size )
  : layer(size)
{

} // pattern_layer::pattern_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Progress all the items in the layer.
 * \param active_area (not used) The active part of the layer.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::pattern_layer::progress
( const region_type& active_area, universe::time_type elapsed_time  )
{
  engine::population::const_iterator it;

  for ( it=m_items.begin(); it!=m_items.end(); ++it )
    it->progress(elapsed_time);
} // pattern_layer::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visible sprites of this layer.
 * \param visuals (out) The sprites in the visible area, and their positions.
 * \param visible_area The visible part of the layer.
 */
void bear::pattern_layer::do_get_visual
( std::list<engine::scene_visual>& visuals,
  const universe::rectangle_type& visible_area ) const
{
  engine::population::const_iterator it;

  for ( it=m_items.begin(); it!=m_items.end(); ++it )
    {
      std::list<engine::scene_visual> local_visuals;
      it->get_visual( local_visuals );

      repeat_visual( visuals, local_visuals, visible_area );
    }
} // pattern_layer::do_get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item in the layer.
 * \param taht The item to add.
 */
void bear::pattern_layer::do_add_item( engine::base_item& that )
{
  m_items.insert(&that);
} // pattern_layer::do_add_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the layer.
 * \param that The item to remove.
 */
void bear::pattern_layer::do_remove_item( engine::base_item& that )
{
  m_items.kill(&that);
} // pattern_layer::do_remove_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the layer without deleting it.
 * \param that The item to remove.
 */
void bear::pattern_layer::do_drop_item( engine::base_item& that )
{
  m_items.drop(&that);
} // pattern_layer::do_drop_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Repeat the sprites from a list of visuals.
 * \param visuals (out) The sprites in the visible area, and their positions.
 * \param local_visuals The sprite took from the items in the layer.
 * \param visible_area The visible part of the layer.
 */
void bear::pattern_layer::repeat_visual
( std::list<engine::scene_visual>& visuals,
  const std::list<engine::scene_visual>& local_visuals,
  const universe::rectangle_type& visible_area ) const
{
  std::list<engine::scene_visual>::const_iterator it;

  for ( it=local_visuals.begin(); it!=local_visuals.end(); ++it )
    repeat_sprite( visuals, *it, visible_area );
} // pattern_layer::repeat_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Repeat the sprite of a visual on the screen.
 * \param visuals (out) The sprites in the visible area, and their positions.
 * \param v The visual to repeat.
 * \param visible_area The visible part of the layer.
 */
void bear::pattern_layer::repeat_sprite
( std::list<engine::scene_visual>& visuals, const engine::scene_visual& v,
  const universe::rectangle_type& visible_area ) const
{
  claw::math::coordinate_2d<int> v_pos;
  const claw::math::coordinate_2d<unsigned int> v_size =
    v.scene_element.get_bounding_box().size();
  const unsigned int x_count = visible_area.width()  / v_size.x + 2;
  const unsigned int y_count = visible_area.height() / v_size.y + 2;

  v_pos.x = visible_area.left() - (int)visible_area.left() % v_size.x;

  for (unsigned int x=0; x!=x_count; ++x)
    {
      v_pos.y = visible_area.bottom() - (int)visible_area.bottom() % v_size.y;

      for (unsigned int y=0; y!=y_count; ++y)
        {
          engine::scene_visual new_v(v);
          new_v.scene_element.set_position(v_pos);
          visuals.push_front(new_v);

          v_pos.y += v_size.y;
        }

      v_pos.x += v_size.x;
    }
} // pattern_layer::repeat_sprite()
