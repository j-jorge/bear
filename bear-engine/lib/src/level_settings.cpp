/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::level_settings class.
 * \author Julien Jorge
 */
#include "bear/generic_items/level_settings.hpp"

#include "bear/engine/level_globals.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param settings The instance through which we set the parameters.
 */
bear::level_settings::loader::loader( engine::level_globals& settings )
  : super("level_settings"), m_settings(settings)
{

} // level_settings::loader::loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a copy of this instance.
 */
bear::level_settings::loader*
bear::level_settings::loader::clone() const
{
  return new loader( *this );
} // level_settings::loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::level_settings::loader::set_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "full_volume_distance" )
    m_settings.set_full_volume_distance( value );
  else if ( name == "silence_distance" )
    m_settings.set_silence_distance( value );
  else
    result = super::set_field(name, value);

  return result;
} // level_settings::loader::set_field()





BASE_ITEM_EXPORT( level_settings, bear )


/*----------------------------------------------------------------------------*/
/**
 * \brief Initializes the item.
 */
void bear::level_settings::build()
{
  kill();
} // level_settings::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::level_settings::populate_loader_map( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( loader( get_level_globals() ) );
} // level_settings::populate_loader_map()
