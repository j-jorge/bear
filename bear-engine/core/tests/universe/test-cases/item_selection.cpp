#include "bear/universe/physical_item.hpp"
#include "bear/universe/internal/item_selection.hpp"

#define BOOST_TEST_MODULE bear::universe::internal/item_selection
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( initial_state )
{
  bear::universe::physical_item item;

  BOOST_CHECK( !item.get_world_progress_structure().is_selected() );
}

BOOST_AUTO_TEST_CASE( select_item )
{
  bear::universe::internal::item_list items;
  bear::universe::physical_item item;

  bear::universe::internal::select_item( items, &item );
  BOOST_CHECK_EQUAL( items.size(), 1 );
  BOOST_CHECK_EQUAL( items.front(), &item );
  BOOST_CHECK( item.get_world_progress_structure().is_selected() );
}

BOOST_AUTO_TEST_CASE( unselect_item )
{
  bear::universe::internal::item_list items;
  bear::universe::physical_item item;

  bear::universe::internal::select_item( items, &item );
  bear::universe::internal::unselect_item( items, items.begin() );
  BOOST_CHECK( items.empty() );
  BOOST_CHECK( !item.get_world_progress_structure().is_selected() );
}
