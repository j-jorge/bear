/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::message_factory class.
 * \author Julien Jorge
 */
#include "engine/network/message_factory.hpp"

/*----------------------------------------------------------------------------*/
bear::engine::message_factory::factory_type*
bear::engine::message_factory::s_factory_instance(NULL);

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the single instance of the factory.
 */
bear::engine::message_factory::factory_type&
bear::engine::message_factory::get_instance()
{
  if ( s_factory_instance == NULL )
    s_factory_instance = new factory_type;

  return *s_factory_instance;
} // message_factory::get_instance()
