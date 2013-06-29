/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This toggle transmits its status to a group of other toggles.
 * \author Julien Jorge
 */
#ifndef __BEAR_TOGGLE_GROUP_HPP__
#define __BEAR_TOGGLE_GROUP_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/item_with_toggle.hpp"
#include "universe/derived_item_handle.hpp"

#include "generic_items/class_export.hpp"

#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief This toggle transmits its status to a group of other toggles.
   *
   * \remark If the state of the toggles in the group change independently from
   * the state of the group, the status of the group will not reflect the status
   * of the sub toggles. So, these sub toggles should have a infinite delay
   * before being automatically turned off and should not be modified by
   * anything else than the group.
   *
   * The following fields are supported by this item:
   *  - toggles (list of toggles), the toggles to which we transmit the status
   *    (default=none),
   *  - any field supported by the parent class.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT toggle_group:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(toggle_group);

    /** \brief The type of the parent class. */
    typedef engine::item_with_toggle<engine::base_item> super;

  private:
    /** \brief The type of the handles on the toggles to activate. */
    typedef universe::derived_item_handle<with_toggle> toggle_handle;

    /** \brief The type of the list in which we store the handles. */
    typedef std::list<toggle_handle> handle_list_type;

  public:
    bool set_item_list_field
      ( const std::string& name, const std::vector<base_item*>& value );

    void insert( engine::base_item* t );

  private:
    void on_toggle_on( engine::base_item* activator );
    void on_toggle_off( engine::base_item* activator );

  private:
    /** \brief The toggles to which we transmit the status. */
    handle_list_type m_toggles;

  }; // class toggle_group
} // namespace bear

#endif // __BEAR_TOGGLE_GROUP_HPP__
