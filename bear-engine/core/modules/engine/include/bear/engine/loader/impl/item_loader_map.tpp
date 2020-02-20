/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the template methods of the
 *        bear::engine::item_loader_map class.
 * \author Julien Jorge.
 */

#include "bear/engine/loader/item_loader.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of the item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<typename T>
bool bear::engine::item_loader_map::set_field
( const std::string& name, T value )
{
  std::string prefix;
  std::string suffix;

  split_field_name( name, prefix, suffix );

  typedef loader_map::iterator iterator_type;

  const std::pair<iterator_type, iterator_type> range =
    m_loader.equal_range( prefix );

  iterator_type it = range.first;
  const iterator_type eit = range.second;

  bool applied(false);

  while ( !applied && (it != eit) )
    {
      applied = it->second.set_field( suffix, value );
      ++it;
    }

  if ( !applied )
    applied = m_fallback.set_field( name, value );

  return applied;
} // item_loader_map::set_field()
