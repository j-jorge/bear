/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of changing the position of an item.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_SET_ITEM_POSITION_HPP__
#define __BF_ACTION_SET_ITEM_POSITION_HPP__

#include "bf/history/action_group.hpp"

namespace bf
{
  class item_instance;

  /**
   * \brief The action of changing the position of an item.
   * \author Julien Jorge
   */
  class action_set_item_position:
    public action_group
  {
  public:
    /**
     * \brief Constructor.
     * \param item The item to modify.
     * \param left The new position of the left edge.
     * \param bottom The new position of the bottom edge.
     */
    action_set_item_position( item_instance* item, double left, double bottom );

    wxString get_description() const;

  }; // class action_set_item_position
} // namespace bf

#endif // __BF_ACTION_SET_ITEM_POSITION_HPP__
