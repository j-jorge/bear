/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A item used to have reference point.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_REFERENCE_ITEM_HPP__
#define __BEAR_REFERENCE_ITEM_HPP__

#include "engine/base_item.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A class representing a reference point.
   *
   * \author Sebastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT reference_item:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(reference_item);

  public:
    reference_item();

  }; // class reference_item
} // namespace bear

#endif // __BEAR_REFERENCE_ITEM_HPP__
