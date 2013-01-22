/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::falling_block class.
 * \author Julien Jorge
 */
#include "generic_items/falling_block.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( falling_block, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::falling_block::falling_block()
{
  // arbitrary value
  set_mass(0.1);
} // falling_block::falling_block()
