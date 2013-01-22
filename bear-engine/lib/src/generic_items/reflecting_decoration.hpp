/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An item that refects collised items.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_REFLECTING_DECORATION_HPP__
#define __BEAR_REFLECTING_DECORATION_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/basic_renderable_item.hpp"

#include "generic_items/class_export.hpp"
#include "engine/export.hpp"
#include "universe/derived_item_handle.hpp"

#include <list>

namespace bear
{
  /**
   * \brief An item that refects collised items.
   * \author Sebastien Angibaud
   * The fields of this item are
   *  - any field supported by the parent classes.
   */
  class GENERIC_ITEMS_EXPORT reflecting_decoration:
    public engine::basic_renderable_item<engine::base_item>
  {
    DECLARE_BASE_ITEM(reflecting_decoration);

  public:
    /** \brief The type of the parent class. */
    typedef engine::basic_renderable_item<engine::base_item> super;

    /** \brief The type of the item handle. */
    typedef universe::derived_item_handle< engine::base_item> item_handle;

    /** \brief The type of the list in which we store collised items. */
    typedef std::list< item_handle > items_list;

    /** \brief Compare two base_item on their z_position. */
    struct GENERIC_ITEMS_EXPORT z_item_position_compare
    {
      bool operator()
	( const item_handle& s1,
	  const item_handle& s2 ) const;
    }; // struct z_item_position_compare

  public:
    void progress( bear::universe::time_type elapsed_time );
    virtual void get_visual( std::list<engine::scene_visual>& visuals ) const;

  private:
    /** \brief The list of collised items. */
    items_list m_items_list;
  }; // class reflecting_decoration
} // namespace bear

#endif // __BEAR_REFLECTING_DECORATION_HPP__
