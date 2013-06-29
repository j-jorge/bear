/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class creates a forced movement of type "forced_goto", applies it
 *        to itself then die.
 * \author Julien Jorge
 */
#ifndef __BEAR_FORCED_GOTO_CREATOR_HPP__
#define __BEAR_FORCED_GOTO_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "universe/forced_movement/forced_goto.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class creates a forced movement of type "forced_goto", applies
   *        it to itself then die.
   *
   * The fields of this item are
   *  - \a target.x: (real) \b [required] \c the X-position to reach,
   *  - \a target.y: (real) \b [required] \c the Y-position to reach,
   *  - \a duration: (real) \c the total time to reach the target (default = 1),
   *  - \a acceleration_time: (real) \c the duration of the acceleration and
   *    deceleration (default = duration / 2).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT forced_goto_creator:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(forced_goto_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    bool set_real_field( const std::string& name, double value );

    void build();

  private:
    /** \brief The movement finally applied. */
    universe::forced_goto m_movement;

  }; // class forced_goto_creator
} // namespace bear

#endif // __BEAR_FORCED_GOTO_CREATOR_HPP__
