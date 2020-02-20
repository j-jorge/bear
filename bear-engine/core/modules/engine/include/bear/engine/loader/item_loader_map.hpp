/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The item_loader_map class associates some item_loader to the prefixes
 *        of their field names.
 * \author Julien Jorge
 */
#ifndef __ENGINE_ITEM_LOADER_MAP_HPP__
#define __ENGINE_ITEM_LOADER_MAP_HPP__

#include <map>
#include <string>

#include "bear/engine/class_export.hpp"
#include "bear/engine/loader/item_loader.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The item_loader_map class associates some item_loader to the
     *        prefixes of their field names.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT item_loader_map
    {
    private:
      /** The type of the map in which the loaders are stored. */
      typedef std::multimap<std::string, item_loader> loader_map;

    public:
      explicit item_loader_map( const item_loader& fallback );

      void insert( const item_loader& loader );
      void insert( const std::string& prefix, const item_loader& loader );

      template<typename T>
      bool set_field( const std::string& name, T value );

    private:
      bool split_field_name
        ( const std::string& name, std::string& prefix,
          std::string& suffix ) const;
      
    private:
      /** The loaders and their prefixes. */
      loader_map m_loader;

      /** The loader that receives the fields' values if no loader has been
          found. */
      item_loader m_fallback;

    }; // class item_loader_map

  } // namespace engine
} // namespace bear

#include "bear/engine/loader/impl/item_loader_map.tpp"

#endif // __ENGINE_ITEM_LOADER_MAP_HPP__
