/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Base class for the proxy used to instanciate layers.
 * \author Julien Jorge
 */
#ifndef __ENGINE_LAYER_CREATOR_HPP__
#define __ENGINE_LAYER_CREATOR_HPP__

#include "universe/types.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class layer;

    /**
     * \brief Base class for layer creators, classes that instanciate layers.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT layer_creator
    {
    public:
      virtual ~layer_creator() { }

      virtual layer* create( const universe::size_box_type& s ) const = 0;

    }; // class layer_creator

  } // namespace engine
} // namespace bear

#endif // __ENGINE_LAYER_CREATOR_HPP__
