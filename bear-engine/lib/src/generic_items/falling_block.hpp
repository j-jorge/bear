/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This block has a small mass, so it falls.
 * \author Julien Jorge
 */
#ifndef __BEAR_FALLING_BLOCK_HPP__
#define __BEAR_FALLING_BLOCK_HPP__

#include "generic_items/base_visible_block.hpp"
#include "universe/collision_event/align_take_speed.hpp"

#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This block has a small mass, so it falls.
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT falling_block :
    public base_visible_block<universe::event_generator_align_take_speed>
  {
  public:
    falling_block();

  }; // class falling_block
} // namespace bear

#endif // __BEAR_FALLING_BLOCK_HPP__
