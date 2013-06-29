/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief  An item that creates some items when it is activated.
 * \author Sébastien Angibaud
 */
#ifndef __BEAR_ITEM_CREATOR_HPP__
#define __BEAR_ITEM_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/item_with_toggle.hpp"
#include "engine/export.hpp"

#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief An item that creates some items when it is activated.
   *
   * The custom fields of this class are:
   * - items (list of reference item) The list of item to create (required).
   * - any field supported by the parent class.
   *
   * \author Sébastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT item_creator:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(item_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_toggle<engine::base_item> super;

  public:
    ~item_creator();

    void build();

    bool set_item_list_field
    ( const std::string& name, const std::vector<base_item*>& value );

    bool is_valid() const;

  private:
    void on_toggle_on( base_item* activator );

  private:
    /** \brief Items that are created. */
    std::vector<engine::base_item*> m_items;
  }; // class item_creator
} // namespace bear

#endif // __BEAR_ITEM_CREATOR_HPP__
