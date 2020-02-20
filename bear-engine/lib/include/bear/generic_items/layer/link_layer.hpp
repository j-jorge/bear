/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This layer displays the links between items.
 * \author Julien Jorge
 */
#ifndef __BEAR_LINK_LAYER_HPP__
#define __BEAR_LINK_LAYER_HPP__

#include "bear/engine/layer/base_debugging_layer.hpp"

#include "bear/generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This layer displays the links between items.
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT link_layer:
    public engine::base_debugging_layer
  {
  private:
    /** \brief The points defining a link. */
    typedef std::vector<bear::visual::position_type> link_type;

  public:
    link_layer();

  private:
    void render
    ( scene_element_list& e, const bear::visual::position_type& delta ) const;

    void progress
    ( const item_list& items, bear::universe::time_type elapsed_time );

  private:
    /** \brief The links to render. */
    std::list<link_type> m_links;

  }; // class link_layer
} // namespace bear

#endif // __BEAR_LINK_LAYER_HPP__
