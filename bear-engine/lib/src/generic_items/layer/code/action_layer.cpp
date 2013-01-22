/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::action_layer class.
 * \author Julien Jorge
 */
#include "generic_items/layer/action_layer.hpp"

#include "engine/level_globals.hpp"
#include "engine/layer/export.hpp"

LAYER_EXPORT( action_layer, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param size The size of the layer/world.
 */
bear::action_layer::action_layer( const universe::size_box_type& size )
  : layer(size), m_world(size)
{

} // action_layer::action_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call the "start" method of all items.
 */
void bear::action_layer::start()
{
  m_world.start();
} // action_layer::start()

/*----------------------------------------------------------------------------*/
/**
 * \brief Progress all items in the active area.
 * \param active_area The area containing active items.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::action_layer::progress
( const region_type& active_area, universe::time_type elapsed_time )
{
  m_world.progress_entities( active_area, elapsed_time );
} // action_layer::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprites of the items in the visible area.
 * \param visuals (out) The sprites in the visible area, and their positions.
 * \param visible_area The visible part of the layer.
 */
void bear::action_layer::do_get_visual
( std::list<engine::scene_visual>& visuals,
  const universe::rectangle_type& visible_area ) const
{
  m_world.get_visual( visuals, visible_area );
} // action_layer::do_get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item in the layer.
 * \param that The item to add.
 */
void bear::action_layer::do_add_item( engine::base_item& that )
{
  if ( that.is_fixed() )
    static_item( that );
  else
    mobile_item( that );
} // action_layer::do_add_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the layer.
 * \param that The item to remove.
 */
void bear::action_layer::do_remove_item( engine::base_item& that )
{
  m_world.release_item( &that );
} // action_layer::do_remove_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the layer without deleting it.
 * \param that The item to remove.
 */
void bear::action_layer::do_drop_item( engine::base_item& that )
{
  m_world.drop_item( &that );
} // action_layer::do_drop_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a static item in the layer.
 * \param that The item to add.
 */
void bear::action_layer::static_item( engine::base_item& that )
{
  CLAW_PRECOND( that.is_valid() );
  CLAW_PRECOND( that.get_mass() == std::numeric_limits<double>::infinity() );

  m_world.add_static( &that );
} // action_layer::static_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an entity in the layer.
 * \param that The entity to add.
 */
void bear::action_layer::mobile_item( engine::base_item& that )
{
  CLAW_PRECOND( that.is_valid() );
  m_world.register_item( &that );
} // action_layer::mobile_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the world.
 */
bear::engine::world* bear::action_layer::do_get_world()
{
  return &m_world;
} // action_layer::do_get_world()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the world.
 */
const bear::engine::world* bear::action_layer::do_get_world() const
{
  return &m_world;
} // action_layer::do_get_world()
