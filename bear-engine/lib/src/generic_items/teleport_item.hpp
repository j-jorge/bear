/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class teleports items in camera.
 * \author Sébastien Angibaud
 */
#ifndef __BEAR_TELEPORT_ITEM_HPP__
#define __BEAR_TELEPORT_ITEM_HPP__

#include "engine/item_brick/item_with_toggle.hpp"
#include "engine/base_item.hpp"
#include "engine/export.hpp"

#include "generic_items/class_export.hpp"

#include "universe/derived_item_handle.hpp"

namespace bear
{
  /**
   * \brief This item teleports object in camera.
   *
   * The custom fields of this class are :
   * - \a reference_point: The item for which center of mass is used 
   *      for teleportation.(required),
   * - any field supported by the parent class.
   *
   * \author Sébastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT teleport_item:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(teleport_item);

  private:
    /** \brief The type of an handle on a object. */
    typedef universe::derived_item_handle<base_item> handle_type;

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_toggle<engine::base_item> super;

  public:
    teleport_item();

    bool set_item_field
      ( const std::string& name, bear::engine::base_item* value );
    bool is_valid() const;
    bear::universe::position_type get_gap() const;

  protected:
    void on_toggle_on( engine::base_item* activator );

  private:
    /** \brief The reference item for teleportation. */
    handle_type m_reference;
  }; // class teleport_item
} // namespace bear

#endif // __BEAR_TELEPORT_ITEM_HPP__
