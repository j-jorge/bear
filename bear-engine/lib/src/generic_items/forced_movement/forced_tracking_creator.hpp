/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class creates a forced movement of type "forced_tracking",
 *        applies it to itself then die.
 * \author Julien Jorge
 */
#ifndef __BEAR_FORCED_TRACKING_CREATOR_HPP__
#define __BEAR_FORCED_TRACKING_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/forced_movement_creator_with_reference.hpp"
#include "universe/forced_movement/forced_tracking.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class creates a forced movement of type "forced_tracking",
   *        applies it to itself then die.
   *
   * The fields of this item are
   *  - \a target: (item) \b [required] \c the item to follow,
   *  - \a duration: (real) \c the total time of the movement (default = inf.),
   *  - \a dist_x: (real) \c the distance to keep on the X axis
   *    (default = auto),
   *  - \a dist_y: (real) \c the distance to keep on the Y axis
   *    (default = auto),
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT forced_tracking_creator:
    public engine::forced_movement_creator_with_reference
    < engine::base_item, universe::forced_tracking >
  {
    DECLARE_BASE_ITEM(forced_tracking_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::forced_movement_creator_with_reference
    < engine::base_item, universe::forced_tracking > super;

  public:
    bool set_real_field( const std::string& name, double value );
  }; // class forced_tracking_creator
} // namespace bear

#endif // __BEAR_FORCED_TRACKING_CREATOR_HPP__
