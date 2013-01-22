/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class applies a forced movement to several clones of an item and
 *        places them at a constant time interval from each other, then it dies.
 * \author Julien Jorge
 */
#ifndef __BEAR_REPEATED_FORCED_MOVEMENT_HPP__
#define __BEAR_REPEATED_FORCED_MOVEMENT_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/forced_movement_repeater.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class applies a forced movement to several clones of an item
   *        and places them at a constant time interval from each other, then it
   *        dies.
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT repeated_forced_movement:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(repeated_forced_movement);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    bool is_valid() const;
    void build();

  protected:
    void populate_loader_map( engine::item_loader_map& m );

  private:
    /** The object that will apply the forced movement. */
    engine::forced_movement_repeater m_movement;

  }; // class repeated_forced_movement
} // namespace bear

#endif // __BEAR_REPEATED_FORCED_MOVEMENT_HPP__
