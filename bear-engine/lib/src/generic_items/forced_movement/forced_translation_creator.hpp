/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class creates a forced movement of type "forced_translation",
 *        applies it to itself then die.
 * \author Julien Jorge
 */
#ifndef __BEAR_FORCED_TRANSLATION_CREATOR_HPP__
#define __BEAR_FORCED_TRANSLATION_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "universe/forced_movement/forced_translation.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class creates a forced movement of type "forced_translation",
   *        applies it to itself then die.
   *
   * The fields of this item are
   *  - \a slant: (real) \c the angle applied to the item (default = 0),
   *  - \a force_angle: (bool) \c tell (true) if we apply the \a angle as is or
   *    (false) if \a angle is added to the angle of the item
   *    (default = false),
   *  - \a duration: (real) \c the total time of the movement (default = inf.),
   *  - \a speed_x: (real) \c the speed of the movement on the X axis
   *    (default = 0),
   *  - \a speed_y: (real) \c the speed of the movement on the Y axis
   *    (default = 0).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT forced_translation_creator:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(forced_translation_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    bool set_real_field( const std::string& name, double value );
    bool set_bool_field( const std::string& name, bool value );

    void build();

  private:
    /** \brief The movement finally applied. */
    universe::forced_translation m_movement;

  }; // class forced_translation_creator
} // namespace bear

#endif // __BEAR_FORCED_TRANSLATION_CREATOR_HPP__
