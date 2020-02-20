/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of resizing an item.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_SET_ITEM_SIZE_HPP__
#define __BF_ACTION_SET_ITEM_SIZE_HPP__

#include "bf/history/action_group.hpp"

namespace bf
{
  class item_instance;

  /**
   * \brief The action of resizing an item.
   * \author Julien Jorge
   */
  class action_set_item_size:
    public action_group
  {
  public:
    /**
     * \brief Constructor.
     * \param item The item to modify.
     * \param width The new width of the item.
     * \param height The new height of the item.
     */
    action_set_item_size
    ( item_instance* item, double width, double height );

    wxString get_description() const;

  }; // class action_set_item_size
} // namespace bf

#endif // __BF_ACTION_SET_ITEM_SIZE_HPP__
