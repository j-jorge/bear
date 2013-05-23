/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of moving the selected items.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_MOVE_SELECTION_HPP__
#define __BF_ACTION_MOVE_SELECTION_HPP__

#include "bf/item_selection.hpp"
#include "bf/history/action_group.hpp"

namespace bf
{
  /**
   * \brief The action of moving the selected items.
   * \author Julien Jorge
   */
  class action_move_selection:
    public action_group
  {
  public:
    action_move_selection
    ( const item_selection& selection, double dx, double dy );

    item_selection get_selection() const;

  private:
    /** \brief The items moved by this action. */
    item_selection m_selection;

  }; // class action_move_selection
} // namespace bf

#endif // __BF_ACTION_MOVE_SELECTION_HPP__
