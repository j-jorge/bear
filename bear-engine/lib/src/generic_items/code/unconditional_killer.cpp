/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::unconditional_killer class.
 * \author Julien Jorge
 */
#include "generic_items/unconditional_killer.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( unconditional_killer, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::unconditional_killer::unconditional_killer()
{
  set_phantom(true);
} // unconditional_killer::unconditional_killer()

/*----------------------------------------------------------------------------*/
/**
 * \brief An item collides with this. Kill him.
 * \param that The colliding item.
 * \param info Some informations about the collision.
 */
void bear::unconditional_killer::collision
( engine::base_item& that, universe::collision_info& info )
{
  that.kill();
} // unconditional_killer::collision()
