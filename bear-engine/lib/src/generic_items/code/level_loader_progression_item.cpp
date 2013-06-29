/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::level_loader_progression_item class.
 * \author Julien Jorge
 */
#include "generic_items/level_loader_progression_item.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( level_loader_progression_item, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the components of the layer.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::level_loader_progression_item::progress
( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  const unsigned int item_index  = get_item_index();
  const unsigned int items_count = get_items_count();

  m_item_bar.set_width( item_index * (unsigned int)get_width() / items_count );
} // level_loader_progression_item::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprite representing the item.
 * \param visuals (out) The sprites of the item, and their positions.
 */
void bear::level_loader_progression_item::get_visual
( std::list<engine::scene_visual>& visuals ) const
{
  add_visual( m_item_bar, visuals );
} // level_loader_progression_item::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type sprite.
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::level_loader_progression_item::set_sprite_field
( const std::string& name, const visual::sprite& value )
{
  bool result = true;

  if ( name == "level_loader_progression_item.sprite" )
    m_item_bar = value;
  else
    result = super::set_sprite_field( name, value );

  return result;
} // level_loader_progression_item::set_sprite_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is well initialised.
 */
bool bear::level_loader_progression_item::is_valid()
{
  return m_item_bar.is_valid() && super::is_valid();
} // level_loader_progression_item::is_valid()
