/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of deleting the selected items.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_DELETE_SELECTION_HPP__
#define __BF_ACTION_DELETE_SELECTION_HPP__

#include "bf/history/action_group.hpp"

#include <vector>

namespace bf
{
  /**
   * \brief The action of deleting the selected items.
   * \author Julien Jorge
   */
  class action_delete_selection:
    public action_group
  {
  public:
    action_delete_selection
    ( const gui_level& lvl, std::vector<std::size_t> layers );

  private:
    void add_actions_for_layer( const gui_level& lvl, std::size_t layer_index );

  }; // class action_delete_selection
} // namespace bf

#endif // __BF_ACTION_DELETE_SELECTION_HPP__
