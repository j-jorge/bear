#pragma once

#include "universe/physical_item.hpp"
#include "test/universe/item_function_call.hpp"

namespace test
{
  namespace universe
  {
    class item_call_tracker:
      public bear::universe::physical_item
    {
    public:
      explicit item_call_tracker( item_function_call& calls );
      
      void add_dependent_item( bear::universe::physical_item& item );
      
    private:
      void time_step( bear::universe::time_type ) override;
      void move( bear::universe::time_type ) override;
      void get_dependent_items
      ( bear::universe::physical_item::item_list& d ) const override;
      
    private:
      item_function_call& m_calls;
      bear::universe::physical_item::item_list m_dependent_items;
    };
  }
}
