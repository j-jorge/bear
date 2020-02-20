/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::repeated_forced_movement class.
 * \author Julien Jorge
 */
#include "bear/generic_items/forced_movement/repeated_forced_movement.hpp"

#include "bear/engine/item_brick/loader/forced_movement_repeater_loader.hpp"

BASE_ITEM_EXPORT( repeated_forced_movement, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is well initialized.
 */
bool bear::repeated_forced_movement::is_valid() const
{
  return m_movement.is_valid() && super::is_valid();
} // repeated_forced_movement::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::repeated_forced_movement::build()
{
  super::build();

  m_movement.give_movement();
  
  kill();
} // repeated_forced_movement::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::repeated_forced_movement::populate_loader_map
( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( engine::forced_movement_repeater_loader(m_movement) );
} // repeated_forced_movement::populate_loader_map()
