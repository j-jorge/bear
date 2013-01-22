/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Macros and function useful for layer exportat.
 * \author Julien Jorge
 */
#ifndef __ENGINE_LAYER_EXPORT_HPP__
#define __ENGINE_LAYER_EXPORT_HPP__

#include "engine/layer/layer_factory.hpp"
#include "engine/layer/typed_layer_creator.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Export a function instanciating a class inheriting from
 *        engine::layer_creator.
 *
 * \param class_name The name of the layer class to instanciate.
 * \param env The full namespace of this class.
 *
 * The env::class_name class must have a constructor taking a const
 * universe::size_box_type& as parameter.
 */
#define LAYER_EXPORT( class_name, env )                                 \
  static bool class_name ## _registered =                               \
    bear::engine::layer_factory::get_instance().register_type           \
    < bear::engine::typed_layer_creator<env::class_name> >              \
  ( #class_name );

#endif // __ENGINE_LAYER_EXPORT_HPP__
