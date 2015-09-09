#include "test/universe/item_mockup.hpp"

test::universe::item_mockup::item_mockup()
  : time_step_impl( []( bear::universe::time_type ) -> void {} )
{

}

void test::universe::item_mockup::time_step
( bear::universe::time_type elapsed_time)
{
  time_step_impl( elapsed_time );
}

void test::universe::item_mockup::collision
( bear::universe::collision_info& info )
{
  collision_impl( info );
}

