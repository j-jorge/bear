/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class creates a forced movement of type "forced_stay_around",
 *        applies it to itself then die.
 * \author Julien Jorge
 */
#ifndef __BEAR_FORCED_STAY_AROUND_CREATOR_HPP__
#define __BEAR_FORCED_STAY_AROUND_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/forced_movement_creator_with_reference.hpp"
#include "universe/forced_movement/forced_stay_around.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class creates a forced movement of type "forced_stay_around",
   *        applies it to itself then die.
   *
   * The fields of this item are
   *  - \a target: (item) \b [required] \c the item around which we stay,
   *  - \a max_angle: (real) \c the maximum angle applied to the item
   *    (default = inf.),
   *  - \a apply_angle: (bool) \c tell (true) if we apply the \a angle of the
   *    movement to the moving item (default = false),
   *  - \a duration: (real) \c the total time of the movement (default = inf.),
   *  - \a speed: (real) \c the constant speed of the item (default = inf.),
   *  - \a max_distance: (real) \c the maximum allowed distance to the reference
   *    item (default = inf.).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT forced_stay_around_creator:
    public engine::forced_movement_creator_with_reference
    < engine::base_item, universe::forced_stay_around >
  {
    DECLARE_BASE_ITEM(forced_stay_around_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::forced_movement_creator_with_reference
    < engine::base_item, universe::forced_stay_around > super;

  public:
    bool set_real_field( const std::string& name, double value );
    bool set_bool_field( const std::string& name, bool value );
  }; // class forced_stay_around_creator
} // namespace bear

#endif // __BEAR_FORCED_STAY_AROUND_CREATOR_HPP__
