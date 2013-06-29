/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class removes the forced movement to an item when activated.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_FORCED_MOVEMENT_DESTRUCTOR_TOGGLE_HPP__
#define __BEAR_FORCED_MOVEMENT_DESTRUCTOR_TOGGLE_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/item_with_toggle.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class remove the forced movement to an item when activated.
   *
   * The fields of this item are
   *  - \a actor: (item list) \b the item to which the movement is removed
   *    (default = none),
   *
   * \author Sebastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT forced_movement_destructor_toggle:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(forced_movement_destructor_toggle);

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_toggle<engine::base_item> super;

  public:
    forced_movement_destructor_toggle();

    bool set_item_list_field
    ( const std::string& name, const std::vector<engine::base_item*>& value );

    bool is_valid() const;

    void on_toggle_on( engine::base_item* activator );

  private:
    /** \brief The items to move. */
    std::vector<universe::item_handle> m_actor;
  }; // class forced_movement_destructor_toggle
} // namespace bear

#endif // __BEAR_FORCED_MOVEMENT_DESTRUCTOR_TOGGLE_HPP__
