/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This layer displays the a colored box representing the physical
 *        attributes of the items.
 * \author Julien Jorge
 */
#ifndef __BEAR_PHYSICS_LAYER_HPP__
#define __BEAR_PHYSICS_LAYER_HPP__

#include "engine/layer/base_debugging_layer.hpp"

#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This layer displays the a colored box representing the physical
   *        attributes of the items.
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT physics_layer:
    public engine::base_debugging_layer
  {
  public:
    physics_layer();

  private:
    void render
    ( scene_element_list& e, const bear::visual::position_type& delta ) const;

    void progress
    ( const item_list& items, bear::universe::time_type elapsed_time );

  private:
    void draw_box
    ( scene_element_list& e, const bear::visual::position_type& delta,
      const bear::universe::physical_item& item ) const;

    void on_show();

  private:
    /** \brief The items to render. */
    item_list m_items;

  }; // class physics_layer
} // namespace bear

#endif // __BEAR_PHYSICS_LAYER_HPP__
