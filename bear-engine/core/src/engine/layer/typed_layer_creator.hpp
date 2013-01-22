/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class that instanciate a layer of a given type.
 * \author Julien Jorge
 */
#ifndef __ENGINE_TYPED_LAYER_CREATOR_HPP__
#define __ENGINE_TYPED_LAYER_CREATOR_HPP__

#include "engine/layer/layer_creator.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief A class that instanciate a layer of a given type.
     * \author Julien Jorge
     */
    template<typename LayerType>
    class typed_layer_creator:
      public layer_creator
    {
    public:
      virtual LayerType* create( const universe::size_box_type& s ) const;

    }; // class typed_layer_creator

  } // namespace engine
} // namespace bear

#include "engine/layer/impl/typed_layer_creator.tpp"

#endif // __ENGINE_TYPED_LAYER_CREATOR_HPP__
