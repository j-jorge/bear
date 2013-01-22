/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::reference_item class.
 * \author Sebastien Angibaud
 */
#include "generic_items/reference_item.hpp"
#include "engine/export.hpp"

BASE_ITEM_EXPORT( reference_item, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::reference_item::reference_item()
{
  set_phantom(true);
  set_artificial(true);
  set_can_move_items(false);
} // reference_item::reference_item()
