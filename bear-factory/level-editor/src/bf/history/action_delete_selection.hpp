/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of deleting the selected items.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_DELETE_SELECTION_HPP__
#define __BF_ACTION_DELETE_SELECTION_HPP__

#include "bf/history/action_group.hpp"

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
    /**
     * \brief Constructor.
     * \param lvl The level in which we take the selection.
     */
    action_delete_selection( const gui_level& lvl );

    wxString get_description() const;

  }; // class action_delete_selection
} // namespace bf

#endif // __BF_ACTION_DELETE_SELECTION_HPP__
