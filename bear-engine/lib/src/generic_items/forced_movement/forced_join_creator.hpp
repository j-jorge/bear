/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class creates a forced movement of type "forced_join", applies it
 *        to itself then die.
 * \author Julien Jorge
 */
#ifndef __BEAR_FORCED_JOIN_CREATOR_HPP__
#define __BEAR_FORCED_JOIN_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/forced_movement_creator_with_reference.hpp"
#include "universe/forced_movement/forced_join.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class creates a forced movement of type "forced_join", applies
   *        it to itself then die.
   *
   * The fields of this item are
   *  - \a target: (item) \b [required] \c the target to reach,
   *  - \a duration: (real) \c the total time to reach the target (default = 1).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT forced_join_creator:
    public engine::forced_movement_creator_with_reference
    < engine::base_item, universe::forced_join >
  {
    DECLARE_BASE_ITEM(forced_join_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::forced_movement_creator_with_reference
    < engine::base_item, universe::forced_join > super;

  public:
    bool set_real_field( const std::string& name, double value );
  }; // class forced_join_creator
} // namespace bear

#endif // __BEAR_FORCED_JOIN_CREATOR_HPP__
