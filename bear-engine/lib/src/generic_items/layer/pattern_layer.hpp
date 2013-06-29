/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A layer that fills the screen with an animation.
 * \author Julien Jorge
 */
#ifndef __BEAR_PATTERN_LAYER_HPP__
#define __BEAR_PATTERN_LAYER_HPP__

#include "engine/population.hpp"
#include "engine/layer/layer.hpp"

#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A layer that fills the screen with an animation.
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT pattern_layer:
    public engine::layer
  {
  public:
    pattern_layer( const universe::size_box_type& size );

    void progress
    ( const region_type& active_area, universe::time_type elapsed_time  );

  private:
    void do_get_visual( std::list<engine::scene_visual>& visuals,
                        const universe::rectangle_type& visible_area ) const;

    void do_add_item( engine::base_item& that );
    void do_remove_item( engine::base_item& item );
    void do_drop_item( engine::base_item& item );

    void repeat_visual
    ( std::list<engine::scene_visual>& visuals,
      const std::list<engine::scene_visual>& local_visuals,
      const universe::rectangle_type& visible_area ) const;

    void repeat_sprite
    ( std::list<engine::scene_visual>& visuals, const engine::scene_visual& v,
      const universe::rectangle_type& visible_area ) const;

    void add( engine::base_item* const& who );
    void remove( engine::base_item* const& who );

  private:
    /** \brief The items repeated in the screen. */
    engine::population m_items;

  }; // class pattern_layer
} // namespace bear

#endif // __BEAR_PATTERN_LAYER_HPP__
