/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::natural_forced_movement_creator class.
 * \author Julien Jorge
 */
#include "bear/generic_items/forced_movement/natural_forced_movement_creator.hpp"

#include "bear/universe/forced_movement/natural_forced_movement.hpp"

#include "bear/engine/export.hpp"

BASE_ITEM_EXPORT( natural_forced_movement_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param mvt The movement loaded by this loader.
 */
bear::natural_forced_movement_creator::loader::loader
( universe::natural_forced_movement& mvt )
  : super("natural_forced_movement_creator"), m_movement(mvt)
{

} // natural_forced_movement_creator::loader::loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a copy of this instance.
 */
bear::natural_forced_movement_creator::loader*
bear::natural_forced_movement_creator::loader::clone() const
{
  return new loader( *this );
} // natural_forced_movement_creator::loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::natural_forced_movement_creator::loader::set_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "duration" )
    m_movement.set_total_time(value);
  else
    result = false;

  return result;
} // natural_forced_movement_creator::loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::natural_forced_movement_creator::build()
{
  set_forced_movement(m_movement);
  kill();
} // natural_forced_movement_creator::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::natural_forced_movement_creator::populate_loader_map
( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( loader(m_movement) );
} // natural_forced_movement_creator::populate_loader_map()
