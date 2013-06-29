/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::decorative_toggle class.
 * \author Julien Jorge
 */
#include "generic_items/decorative_toggle.hpp"

BASE_ITEM_EXPORT( decorative_toggle, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::decorative_toggle::decorative_toggle()
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // decorative_toggle::decorative_toggle()
