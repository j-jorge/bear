/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::ambient_sound class.
 * \author Julien Jorge
 */
#include "bear/generic_items/ambient_sound.hpp"

BASE_ITEM_EXPORT( ambient_sound, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::ambient_sound::ambient_sound()
{
  set_can_move_items(false);
  set_phantom(true);
} // ambient_sound::ambient_sound()
