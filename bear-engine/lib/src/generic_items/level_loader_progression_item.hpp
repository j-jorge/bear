/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item that fills a bar while loading a level.
 * \author Julien Jorge
 */
#ifndef __BEAR_LEVEL_LOADER_PROGRESSION_ITEM_HPP__
#define __BEAR_LEVEL_LOADER_PROGRESSION_ITEM_HPP__

#include "generic_items/level_loader_item.hpp"
#include "engine/item_brick/basic_renderable_item.hpp"

#include "generic_items/class_export.hpp"

#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief An item that fills a bar while loading a level.
   *
   * The fields of this class are:
   *  - \a sprite: <sprite> the sprite that grows while the level is loaded.
   *    Required.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT level_loader_progression_item:
    public engine::basic_renderable_item<level_loader_item>
  {
    DECLARE_BASE_ITEM(level_loader_progression_item);

  public:
    /** \brief The type of the parent class. */
    typedef engine::basic_renderable_item<level_loader_item> super;

  public:
    void progress( universe::time_type elapsed_time );
    void get_visual( std::list<engine::scene_visual>& visuals ) const;

    bool set_sprite_field
    ( const std::string& name, const visual::sprite& value );

    bool is_valid();

  private:
    /** \brief The progess bar of the items. */
    visual::sprite m_item_bar;

  }; // class level_loader_progression_item
} // namespace bear

#endif // __BEAR_LEVEL_LOADER_PROGRESSION_ITEM_HPP__
