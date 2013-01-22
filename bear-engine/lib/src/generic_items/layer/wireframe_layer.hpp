/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This layer displays the bounding boxes of the items.
 * \author Julien Jorge
 */
#ifndef __BEAR_WIREFRAME_LAYER_HPP__
#define __BEAR_WIREFRAME_LAYER_HPP__

#include "engine/layer/base_debugging_layer.hpp"

#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This layer displays the bounding boxes if the items.
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT wireframe_layer:
    public engine::base_debugging_layer
  {
  public:
    wireframe_layer();

  private:
    void render
    ( scene_element_list& e, const bear::visual::position_type& delta ) const;

    void progress
    ( const item_list& items, bear::universe::time_type elapsed_time );

  private:
    void draw_box
    ( scene_element_list& e, const bear::visual::position_type& delta,
      const bear::universe::physical_item& item,
      const bear::visual::color_type& color ) const;
    void draw_system
    ( scene_element_list& e, const bear::visual::position_type& delta,
      const bear::universe::physical_item& item,
      const bear::visual::color_type& color ) const;
    void draw_slope
    ( scene_element_list& e, const bear::visual::position_type& delta,
      const bear::universe::physical_item& item,
      const bear::visual::color_type& color ) const;

  private:
    /** \brief The items to render. */
    item_list m_items;

  }; // class wireframe_layer
} // namespace bear

#endif // __BEAR_WIREFRAME_LAYER_HPP__
