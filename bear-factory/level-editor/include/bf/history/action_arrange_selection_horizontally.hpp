/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of uniformizing the horizontal space between the items of a
 *        selection.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_ARRANGE_SELECTION_HORIZONTALLY_HPP__
#define __BF_ACTION_ARRANGE_SELECTION_HORIZONTALLY_HPP__

#include "bf/history/action_group.hpp"

namespace bf
{
  class item_selection;

  /**
   * \brief The action of uniformizing the horizontal space between the items of
   *        a selection.
   * \author Julien Jorge
   */
  class action_arrange_selection_horizontally:
    public action_group
  {
  public:
    action_arrange_selection_horizontally( const item_selection& selection );

  }; // class action_arrange_selection_horizontally
} // namespace bf

#endif // __BF_ACTION_ARRANGE_SELECTION_HORIZONTALLY_HPP__
