/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
#ifndef __UNIVERSE_PHYSICAL_ITEM_FLAGS_HPP__
#define __UNIVERSE_PHYSICAL_ITEM_FLAGS_HPP__

#include "bear/universe/class_export.hpp"

#include <cstdint>

namespace bear
{
  namespace universe
  {
    struct UNIVERSE_EXPORT physical_item_flags
    {
      typedef std::int8_t type;
      
      static constexpr type free_system_angle = 1 << 0;
      static constexpr type can_move_items = 1 << 1;
      static constexpr type phantom = 1 << 2;
      static constexpr type artificial = 1 << 3;
      static constexpr type weak_collisions = 1 << 4;
      static constexpr type global = 1 << 5;
    };
  }
}

#endif
