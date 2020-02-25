/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A factory to instanciate classes inheriting from base_item.
 * \author Julien Jorge
 */
#ifndef __ENGINE_ITEM_FACTORY_HPP__
#define __ENGINE_ITEM_FACTORY_HPP__

#include "bear/engine/class_export.hpp"

#include <claw/factory.hpp>
#include <string>

namespace bear
{
  namespace engine
  {
    class base_item;

    /**
     * \brief A factory to instanciate classes inheriting from base_item.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT item_factory
    {
    public:
      /** \brief The type of the factory. */
      typedef claw::pattern::factory<base_item, std::string> factory_type;

    public:
      static factory_type& get_instance();

    private:
      /** \brief A factory that creates items. */
      static factory_type* s_factory_instance;

    }; // class item_factory

  } // namespace engine
} // namespace bear

#endif // __ENGINE_ITEM_FACTORY_HPP__
