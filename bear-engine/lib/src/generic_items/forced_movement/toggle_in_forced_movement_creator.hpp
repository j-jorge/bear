/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class creates a forced movement of type
 *        "toggle_in_forced_movement", applies it to itself then die.
 * \author Julien Jorge
 */
#ifndef __BEAR_TOGGLE_IN_FORCED_MOVEMENT_CREATOR_HPP__
#define __BEAR_TOGGLE_IN_FORCED_MOVEMENT_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "engine/forced_movement/toggle_in_forced_movement.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class creates a forced movement of type
   *        "toggle_in_forced_movement", applies it to itself then die.
   *
   * The fields of this item are
   *  - \a toggle: (item with toggle) \b [required] \c the toggle to insert in
   *    the forced movement,
   *  - \a use_toggle_duration: (bool) \c tell if the duration of the movement
   *    is the delay of the toggle (default = false),
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT toggle_in_forced_movement_creator:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(toggle_in_forced_movement_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    bool set_bool_field( const std::string& name, bool value );
    bool set_item_field( const std::string& name, engine::base_item* value );
    bool is_valid() const;

    void build();

  private:
    /** \brief The movement finally applied. */
    engine::toggle_in_forced_movement m_movement;

  }; // class toggle_in_forced_movement_creator
} // namespace bear

#endif // __BEAR_TOGGLE_IN_FORCED_MOVEMENT_CREATOR_HPP__
