/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A minimal toggle.
 * \author Julien Jorge
 */
#ifndef __BEAR_TOGGLE_HPP__
#define __BEAR_TOGGLE_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/item_with_toggle.hpp"

#include "generic_items/class_export.hpp"
#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief A minimal toggle.
   * \author Julien Jorge
   *
   * The custom fields of this class are the fields of the parent classes.
   */
  class GENERIC_ITEMS_EXPORT toggle:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(toggle);

  }; // class toggle
} // namespace bear

#endif // __BEAR_TOGGLE_HPP__
