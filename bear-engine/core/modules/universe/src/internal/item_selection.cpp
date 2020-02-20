#include "bear/universe/internal/item_selection.hpp"

#include "bear/universe/physical_item.hpp"

#include <claw/assert.hpp>

bool
bear::universe::internal::select_item( item_list& items, physical_item* it )
{
  if ( it->get_world_progress_structure().is_selected() )
    return false;
  
  items.push_back(it);
  it->get_world_progress_structure().init();
  it->get_world_progress_structure().select();

  return true;
}

void bear::universe::internal::unselect_item
( item_list& items, item_list::iterator it )
{
  CLAW_PRECOND( (*it)->get_world_progress_structure().is_selected() );

  (*it)->get_world_progress_structure().deinit();
  items.erase(it);
}
