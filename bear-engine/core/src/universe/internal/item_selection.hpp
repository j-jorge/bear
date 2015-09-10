#ifndef __UNIVERSE_ITEM_SELECTION_HPP__
#define __UNIVERSE_ITEM_SELECTION_HPP__

#include <vector>

namespace bear
{
  namespace universe
  {
    class physical_item;
    
    namespace internal
    {
      typedef std::vector<physical_item*> item_list;

      bool select_item( item_list& items, physical_item* it );
      void unselect_item( item_list& items, item_list::iterator it );
    }
  }
}

#endif
