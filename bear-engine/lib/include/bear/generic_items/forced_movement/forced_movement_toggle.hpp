/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class applies a forced movement to an item when activated.
 * \author Julien Jorge
 */
#ifndef __BEAR_FORCED_MOVEMENT_TOGGLE_HPP__
#define __BEAR_FORCED_MOVEMENT_TOGGLE_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/engine/item_brick/item_with_toggle.hpp"
#include "bear/engine/item_brick/forced_movement_applicator.hpp"

#include "bear/engine/export.hpp"
#include "bear/generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class applies a forced movement to an item when activated.
   *
   * The fields of this item are
   *  - \a apply_to_activator: (bool) \c tell if the movement has to be applied
   *    to the activator too (default = false),
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT forced_movement_toggle:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(forced_movement_toggle);

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_toggle<engine::base_item> super;

  public:
    forced_movement_toggle();

    bool is_valid() const;

    bool set_bool_field( const std::string& name, bool value );
    void on_toggle_on( engine::base_item* activator );

  protected:
    void populate_loader_map( engine::item_loader_map& m );

  private:
    /** \brief Tell if the movement has to be applied to the activator too. */
    bool m_apply_to_activator;

    /** The object that will apply the forced movement. */
    engine::forced_movement_applicator m_movement;

  }; // class forced_movement_toggle
} // namespace bear

#endif // __BEAR_FORCED_MOVEMENT_TOGGLE_HPP__
