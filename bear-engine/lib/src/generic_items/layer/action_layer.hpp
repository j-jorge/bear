/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The layer where all entities live.
 * \author Julien Jorge
 */
#ifndef __BEAR_ACTION_LAYER_HPP__
#define __BEAR_ACTION_LAYER_HPP__

#include "engine/world.hpp"
#include "engine/layer/layer.hpp"
#include "engine/base_item.hpp"

#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief The layer where all entities live.
   */
  class GENERIC_ITEMS_EXPORT action_layer:
    public engine::layer
  {
  public:
    action_layer( const universe::size_box_type& size );

    virtual void start();

    virtual void progress
    ( const region_type& active_area, universe::time_type elapsed_time  );

  private:
    void do_get_visual( std::list<engine::scene_visual>& visuals,
                        const universe::rectangle_type& visible_area ) const;

    void do_add_item( engine::base_item& that );
    void do_remove_item( engine::base_item& item );
    void do_drop_item( engine::base_item& item );

    void static_item( engine::base_item& that );
    void mobile_item( engine::base_item& that );

    engine::world* do_get_world();
    const engine::world* do_get_world() const;

  private:
    /** \brief The world, and physic rules associated. */
    engine::world m_world;

  }; // class action_layer
} // namespace bear

#endif // __BEAR_ACTION_LAYER_HPP__
