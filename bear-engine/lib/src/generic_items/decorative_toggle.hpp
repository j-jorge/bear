/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A minimal toggle, used only for its animations.
 * \author Julien Jorge
 */
#ifndef __BEAR_DECORATIVE_TOGGLE_HPP__
#define __BEAR_DECORATIVE_TOGGLE_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/basic_renderable_item.hpp"
#include "engine/item_brick/decorated_item_with_toggle.hpp"

#include "generic_items/class_export.hpp"

#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief A minimal toggle, used only for its animations.
   * \author Julien Jorge
   *
   * The custom fields of this class are the fields of the parent classes.
   */
  class GENERIC_ITEMS_EXPORT decorative_toggle:
    public engine::decorated_item_with_toggle
    < engine::basic_renderable_item<engine::base_item> >
  {
    DECLARE_BASE_ITEM(decorative_toggle);

  public:
    decorative_toggle();

  }; // class decorative_toggle
} // namespace bear

#endif // __BEAR_DECORATIVE_TOGGLE_HPP__
