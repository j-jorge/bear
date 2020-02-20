/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::item_loader_map class.
 * \author Julien Jorge.
 */
#include "bear/engine/loader/item_loader_map.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Construcor.
 * \param fallback The loader that receives the fields' values if no loader has
 *        been found.
 */
bear::engine::item_loader_map::item_loader_map( const item_loader& fallback )
  : m_fallback(fallback)
{

} // item_loader_map::item_loader_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a new loader in the map.
 * \param loader The loader to insert.
 *
 * Equivalent to insert( loader.get_name(), loader ).
 */
void bear::engine::item_loader_map::insert( const item_loader& loader )
{
  insert( loader.get_name(), loader );
} // item_loader_map::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a new loader in the map.
 * \param prefix The prefix associated to the loader in the map.
 * \param loader The loader to insert.
 */
void bear::engine::item_loader_map::insert
( const std::string& prefix, const item_loader& loader )
{
  CLAW_PRECOND( m_loader.find(prefix) == m_loader.end() );

  m_loader.insert( loader_map::value_type( prefix, loader ) );
} // item_loader_map::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the part before the first dot and the part after it in a given
 *        string.
 * \param name The string to split.
 * \param prefix (out) The part of \a name before the first dot.
 * \param suffix (out) The part of \a name after the first dot.
 * \return false if the \a name has no dot.
 */
bool bear::engine::item_loader_map::split_field_name
( const std::string& name, std::string& prefix, std::string& suffix ) const
{
  std::string::size_type pos = name.find_first_of('.');

  if ( pos == std::string::npos )
    return false;
  else
    {
      prefix = name.substr( 0, pos );
      suffix = name.substr( pos + 1 );
      return true;
    }
} // item_loader_map::split_field_name()
