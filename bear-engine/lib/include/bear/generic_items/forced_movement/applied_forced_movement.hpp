/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class applies a forced movement to an item then die.
 * \author Julien Jorge
 */
#ifndef __BEAR_APPLIED_FORCED_MOVEMENT_HPP__
#define __BEAR_APPLIED_FORCED_MOVEMENT_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/engine/item_brick/forced_movement_applicator.hpp"

#include "bear/engine/export.hpp"
#include "bear/generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class applies a forced movement to an item then die.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT applied_forced_movement:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(applied_forced_movement);

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
    engine::forced_movement_applicator m_movement;
        
  }; // class applied_forced_movement
} // namespace bear

#endif // __BEAR_APPLIED_FORCED_MOVEMENT_HPP__
