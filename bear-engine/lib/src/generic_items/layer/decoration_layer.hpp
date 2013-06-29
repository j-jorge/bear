/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A decoration layer contains animation and sprites positioned in the
 *        world.
 * \author Julien Jorge
 */
#ifndef __BEAR_DECORATION_LAYER_HPP__
#define __BEAR_DECORATION_LAYER_HPP__

#include "visual/screen.hpp"
#include "universe/static_map.hpp"
#include "engine/layer/layer.hpp"
#include "engine/base_item.hpp"

#include <claw/math.hpp>

#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A decoration layer contains animation and sprites positioned in
   *        the world.
   */
  class GENERIC_ITEMS_EXPORT decoration_layer:
    public engine::layer
  {
  public:
    decoration_layer( const universe::size_box_type& size );
    virtual ~decoration_layer();

    void start();

    void progress
    ( const region_type& active_area, universe::time_type elapsed_time  );

    void log_statistics() const;

  private:
    void do_get_visual( std::list<engine::scene_visual>& visuals,
                        const universe::rectangle_type& visible_area ) const;

    void do_add_item( engine::base_item& item );
    void do_remove_item( engine::base_item& item );
    void do_drop_item( engine::base_item& item );

  private:
    /** \brief All the decorations. */
    universe::static_map<engine::base_item*> m_items;

    /** \brief All global items. */
    std::list<engine::base_item*> m_global_items;

  }; // class decoration_layer
} // namespace bear

#endif // __BEAR_DECORATION_LAYER_HPP__
