/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class creates a forced movement of type "forced_rotation",
 *        applies it to itself then die.
 * \author Julien Jorge
 */
#ifndef __BEAR_FORCED_ROTATION_CREATOR_HPP__
#define __BEAR_FORCED_ROTATION_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/forced_movement_creator_with_reference.hpp"
#include "universe/forced_movement/forced_rotation.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class creates a forced movement of type "forced_rotation",
   *        applies it to itself then die.
   *
   * The fields of this item are
   *  - \a center: (item) \b [required] \c the center of the rotation,
   *  - \a start_angle: (real) the initial angle (default = -3.1415),
   *  - \a end_angle: (real) the final angle (default = 3.1415),
   *  - \a duration: (real) \c the total time needed to go from \a start_angle
   *    to end_angle (default = 1),
   *  - \a radius: (real) the distance of the moving item from the center of the
   *    rotation (default = auto),
   *  - \a loop_back: (bool) tell to play the rotation backward when the end is
   *    reached (default = false),
   *  - \a loops: (unsigned int) do the rotation this number of times
   *    (default = infinity),
   *  - \a apply_angle: (string) tell how the angle of the rotation is applied
   *    to the rotating item. Valid values are "add", "force" and "keep"
   *    (default = "keep").
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT forced_rotation_creator:
    public engine::forced_movement_creator_with_reference
    < engine::base_item, universe::forced_rotation >
  {
    DECLARE_BASE_ITEM(forced_rotation_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::forced_movement_creator_with_reference
    < engine::base_item, universe::forced_rotation > super;

  public:
    bool set_real_field( const std::string& name, double value );
    bool set_bool_field( const std::string& name, bool value );
    bool set_string_field( const std::string& name, const std::string& value );
    bool set_u_integer_field( const std::string& name, unsigned int value );
  }; // class forced_rotation_creator
} // namespace bear

#endif // __BEAR_FORCED_ROTATION_CREATOR_HPP__
