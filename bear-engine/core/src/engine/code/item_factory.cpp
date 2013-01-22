/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::item_factory class.
 * \author Julien Jorge
 */
#include "engine/item_factory.hpp"

/*----------------------------------------------------------------------------*/
bear::engine::item_factory::factory_type*
bear::engine::item_factory::s_factory_instance(NULL);

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the single instance of the factory.
 */
bear::engine::item_factory::factory_type&
bear::engine::item_factory::get_instance()
{
  if ( s_factory_instance == NULL )
    s_factory_instance = new factory_type;

  return *s_factory_instance;
} // item_factory::get_instance()
