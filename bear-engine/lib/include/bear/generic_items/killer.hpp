/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item that kills other items.
 * \author Julien Jorge
 */
#ifndef __BEAR_KILLER_HPP__
#define __BEAR_KILLER_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/engine/item_brick/with_toggle.hpp"
#include "bear/universe/derived_item_handle.hpp"

#include "bear/engine/export.hpp"
#include "bear/generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief An item that kills other items and itself.
   *
   * The valid fields for this item are
   *  - \a kill_activator: (bool) Tell if the activator must be killed too
   *    (default = false),
   *  - \a items: \c (list of items) The items to kill,
   *  - any field supported by the parent classes.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT killer:
    public engine::base_item,
    public engine::with_toggle
  {
    DECLARE_BASE_ITEM(killer);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  private:
    /** \brief The type of the handles on the items to kill. */
    typedef universe::derived_item_handle<engine::base_item> handle_type;

  public:
    killer();

    bool set_item_list_field
    ( const std::string& name, const std::vector<base_item*>& value );
    bool set_bool_field( const std::string& name, bool value );

  private:
    void toggle_on( base_item* activator );

  private:
    /** \brief The list of items to kill. */
    std::vector<handle_type> m_items;

    /** \brief Tell if we kill the activator too. */
    bool m_kill_activator;

  }; // class killer
} // namespace bear

#endif // __BEAR_KILLER_HPP__
