/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A sound in the level.
 * \author Julien Jorge
 */
#ifndef __BEAR_AMBIENT_SOUND_HPP__
#define __BEAR_AMBIENT_SOUND_HPP__

#include "audio/sample.hpp"
#include "engine/base_item.hpp"
#include "engine/item_brick/item_with_toggle.hpp"

#include "generic_items/class_export.hpp"

#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief A sound in the level.
   *
   * The following fields are supported by this item:
   *  - sample (string) [required], the path of the sound to play,
   *  - fadeout (real), fade the sample out during this duration, in seconds,
   *    after the time limit (default = 0).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT ambient_sound:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(ambient_sound);

    /** \brief The type of the parent class. */
    typedef engine::item_with_toggle<engine::base_item> super;

  public:
    ambient_sound();

  }; // class ambient_sound
} // namespace bear

#endif // __BEAR_AMBIENT_SOUND_HPP__
