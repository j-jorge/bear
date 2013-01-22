/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A factory to instanciate classes inheriting from layer_creator.
 * \author Julien Jorge
 */
#ifndef __ENGINE_LAYER_FACTORY_HPP__
#define __ENGINE_LAYER_FACTORY_HPP__

#include "engine/layer/layer_creator.hpp"
#include "engine/class_export.hpp"

#include <claw/factory.hpp>
#include <string>

namespace bear
{
  namespace engine
  {
    /**
     * \brief A factory to instanciate classes inheriting from layer_creator.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT layer_factory
    {
    public:
      /** \brief The type of the factory. */
      typedef claw::pattern::factory<layer_creator, std::string> factory_type;

    public:
      static factory_type& get_instance();

    private:
      /** \brief A factory that creates layers. */
      static factory_type* s_factory_instance;

    }; // class layer_factory

  } // namespace engine
} // namespace bear

#endif // __ENGINE_LAYER_FACTORY_HPP__
