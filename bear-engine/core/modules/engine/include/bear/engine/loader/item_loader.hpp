/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The classes related to the loading of the items in the level files.
 * \author Julien Jorge
 */
#ifndef __ENGINE_ITEM_LOADER_HPP__
#define __ENGINE_ITEM_LOADER_HPP__

#include <string>

#include "bear/engine/class_export.hpp"

#include "bear/audio/sample.hpp"
#include "bear/visual/sprite.hpp"
#include "bear/visual/animation.hpp"

namespace bear
{
  namespace engine
  {
    class item_loader_base;

    /**
     * \brief Base class for the loaders of the items.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT item_loader
    {
    public:
      item_loader();
      item_loader( const item_loader_base& l );
      item_loader( const item_loader& that );
      ~item_loader();

      item_loader& operator=( item_loader that );

      void swap( item_loader& that );

      std::string get_name() const;

      template<typename T>
      bool set_field( const std::string& name, T value );

    private:
      /** The implementation that effectively loads the item. */
      item_loader_base* m_impl;

    }; // class item_loader

  } // namespace engine
} // namespace bear

#include "bear/engine/loader/impl/item_loader.tpp"

#endif // __ENGINE_ITEM_LOADER_HPP__
