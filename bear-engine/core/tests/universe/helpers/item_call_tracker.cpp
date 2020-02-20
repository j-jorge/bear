#include "test/universe/item_call_tracker.hpp"

test::universe::item_call_tracker::item_call_tracker
( item_function_call& calls )
  : m_calls( calls )
{

}

void test::universe::item_call_tracker::add_dependent_item
( bear::universe::physical_item& item )
{
  m_dependent_items.push_back( &item );
}
      
void test::universe::item_call_tracker::time_step( bear::universe::time_type )
{
  m_calls.time_step.push_back( this );
}
  
void test::universe::item_call_tracker::move( bear::universe::time_type )
{
  m_calls.move.push_back( this );
}

void test::universe::item_call_tracker::get_dependent_items
( bear::universe::physical_item::item_list& d ) const
{
  d = m_dependent_items;
}
