/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::typed_layer_creator class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a layer.
 * \param s The size of the layer.
 */
template<typename LayerType>
LayerType* bear::engine::typed_layer_creator<LayerType>::create
( const universe::size_box_type& s ) const
{
  return new LayerType(s);
} // typed_layer_creator::create()
