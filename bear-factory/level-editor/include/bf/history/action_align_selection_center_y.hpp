/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of aligning the center_y of the selected items.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_ALIGN_SELECTION_CENTER_Y_HPP__
#define __BF_ACTION_ALIGN_SELECTION_CENTER_Y_HPP__

#include "bf/history/action_group.hpp"

namespace bf
{
  class item_selection;

  /**
   * \brief The action of aligning the center_y of the selected items.
   * \author Julien Jorge
   */
  class action_align_selection_center_y:
    public action_group
  {
  public:
    action_align_selection_center_y( const item_selection& selection );

  }; // class action_align_selection_center_y
} // namespace bf

#endif // __BF_ACTION_ALIGN_SELECTION_CENTER_Y_HPP__
