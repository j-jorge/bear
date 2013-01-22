/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This layer displays some lines linking the last positions of the
 *        items.
 * \author Julien Jorge
 */
#ifndef __BEAR_RECENT_PATH_LAYER_HPP__
#define __BEAR_RECENT_PATH_LAYER_HPP__

#include "engine/layer/base_debugging_layer.hpp"
#include <map>

#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This layer displays some lines linking the last positions of the
   *        items.
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT recent_path_layer:
    public engine::base_debugging_layer
  {
  private:
    class item_positions:
      public std::vector<bear::universe::position_type>
    {
    public:
      typedef std::vector<bear::universe::position_type> super;

    public:
      item_positions( const bear::universe::physical_item* item );

      bool has_moved( const bear::universe::physical_item* item ) const;

    }; // class item_positions

    typedef std::map< const bear::universe::physical_item*,
                      std::list<item_positions> > item_map;

  public:
    recent_path_layer();

  private:
    void render
    ( scene_element_list& e, const bear::visual::position_type& delta ) const;

    void progress
    ( const item_list& items, bear::universe::time_type elapsed_time );

  private:
    /** \brief The items and their paths. */
    item_map m_items;

  }; // class recent_path_layer
} // namespace bear

#endif // __BEAR_RECENT_PATH_LAYER_HPP__
