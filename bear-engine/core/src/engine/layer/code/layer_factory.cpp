/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::layer_factory class.
 * \author Julien Jorge
 */
#include "engine/layer/layer_factory.hpp"

/*----------------------------------------------------------------------------*/
bear::engine::layer_factory::factory_type*
bear::engine::layer_factory::s_factory_instance(NULL);

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the single instance of the factory.
 */
bear::engine::layer_factory::factory_type&
bear::engine::layer_factory::get_instance()
{
  if ( s_factory_instance == NULL )
    s_factory_instance = new factory_type;

  return *s_factory_instance;
} // layer_factory::get_instance()
