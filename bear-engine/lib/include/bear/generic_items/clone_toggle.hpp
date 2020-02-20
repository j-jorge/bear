/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This toggle creates a clone of an item each time it is turned on.
 * \author Julien Jorge
 */
#ifndef __BEAR_CLONE_TOGGLE_HPP__
#define __BEAR_CLONE_TOGGLE_HPP__

#include "bear/engine/item_brick/item_with_toggle.hpp"
#include "bear/engine/base_item.hpp"
#include "bear/engine/export.hpp"
#include "bear/generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This toggle creates a clone of an item each time it is turned on.
   *
   * The custom fields of this class are :
   * - \a items: list of items, the items to clone (default = none),
   * - any field supported by the parent class.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT clone_toggle:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(clone_toggle);

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_toggle<engine::base_item> super;

  public:
    ~clone_toggle();

    bool set_item_list_field
      ( const std::string& name, const std::vector<engine::base_item*>& value );

  private:
    void on_toggle_on( engine::base_item* activator );

  private:
    /** \brief The items to clone. */
    std::vector<engine::base_item*> m_items;

  }; // class clone_toggle
} // namespace bear

#endif // __BEAR_CLONE_TOGGLE_HPP__
