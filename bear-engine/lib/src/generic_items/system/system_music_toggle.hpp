/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A toggle that mutes or unmutes the music.
 * \author Julien Jorge
 */
#ifndef __BEAR_SYSTEM_MUSIC_TOGGLE_HPP__
#define __BEAR_SYSTEM_MUSIC_TOGGLE_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/item_with_input_listener.hpp"

#include "generic_items/decorative_toggle.hpp"

#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief A toggle that mutes or unmutes the music.
   * \author Julien Jorge
   *
   * The custom fields of this class are the fields of the parent classes.
   */
  class GENERIC_ITEMS_EXPORT system_music_toggle:
    public engine::item_with_input_listener< decorative_toggle >
  {
    DECLARE_BASE_ITEM(system_music_toggle);

  private:
    /** \brief The type of the parent item class. */
    typedef engine::item_with_input_listener< decorative_toggle > super;

  public:
    void progress( universe::time_type elapsed_time );

  private:
    bool mouse_pressed_local
      ( input::mouse::mouse_code button, const universe::position_type& pos );

  }; // class system_music_toggle
} // namespace bear

#endif // __BEAR_SYSTEM_MUSIC_TOGGLE_HPP__
