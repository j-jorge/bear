/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item that quit the game.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_GAME_QUIT_HPP__
#define __BEAR_GAME_QUIT_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/with_toggle.hpp"
#include "universe/derived_item_handle.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief An item that quit the game.
   *
   * The valid fields for this item are
   *  - any field supported by the parent classes.
   *
   * \author Sebastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT game_quit:
    public engine::base_item,
    public engine::with_toggle
  {
    DECLARE_BASE_ITEM(game_quit);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    game_quit();

  private:
    void toggle_on( base_item* activator );
  }; // class game_quit
} // namespace bear

#endif // __BEAR_GAME_QUIT_HPP__
