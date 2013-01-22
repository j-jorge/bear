/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of aligning the bottom of the selected items.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_ALIGN_SELECTION_BOTTOM_HPP__
#define __BF_ACTION_ALIGN_SELECTION_BOTTOM_HPP__

#include "bf/history/action_group.hpp"

namespace bf
{
  /**
   * \brief The action of aligning the bottom of the selected items.
   * \author Julien Jorge
   */
  class action_align_selection_bottom:
    public action_group
  {
  public:
    /**
     * \brief Constructor.
     * \param lvl The level in which we take the selection.
     */
    action_align_selection_bottom( const gui_level& lvl );

    wxString get_description() const;

  }; // class action_align_selection_bottom
} // namespace bf

#endif // __BF_ACTION_ALIGN_SELECTION_BOTTOM_HPP__
