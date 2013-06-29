/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item that kills all colliding items.
 * \author Julien Jorge
 */
#ifndef __BEAR_UNCONDITIONAL_KILLER_HPP__
#define __BEAR_UNCONDITIONAL_KILLER_HPP__

#include "engine/base_item.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief An item that kills all colliding items.
   *
   * The valid fields for this item are
   *  - any field supported by the parent classes.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT unconditional_killer:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(unconditional_killer);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    unconditional_killer();

  protected:
    virtual void collision
      ( engine::base_item& that, universe::collision_info& info );

  }; // class unconditional_killer
} // namespace bear

#endif // __BEAR_UNCONDITIONAL_KILLER_HPP__
