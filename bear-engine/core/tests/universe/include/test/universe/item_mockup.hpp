#pragma once

#include "bear/universe/physical_item.hpp"

#include <functional>

namespace test
{
  namespace universe
  {
    class item_mockup:
      public bear::universe::physical_item
    {
    public:
      item_mockup();

    private:
      void time_step( bear::universe::time_type elapsed_time ) override;
      void collision( bear::universe::collision_info& info ) override;
      
    public:
      std::function< void( bear::universe::time_type ) > time_step_impl;
      std::function< void( bear::universe::collision_info& ) > collision_impl;
    };
  }
}
