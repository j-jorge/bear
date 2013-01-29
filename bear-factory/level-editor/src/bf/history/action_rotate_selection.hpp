/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of rotating the selected items.
 * \author Sebastien Angibaud
 */
#ifndef __BF_ACTION_ROTATE_SELECTION_HPP__
#define __BF_ACTION_ROTATE_SELECTION_HPP__

#include "bf/history/action_group.hpp"

namespace bf
{
  /**
   * \brief The action of rotating the selected items.
   * \author Sebastien Angibaud
   */
  class action_rotate_selection:
    public action_group
  {
  public:
    /**
     * \brief Constructor.
     * \param lvl The level in which we take the selection.
     * \param clockwise Indicates if the rotation is clockwise.
     */
    action_rotate_selection( const gui_level& lvl, bool clockwise );

    wxString get_description() const;

  }; // class action_rotate_selection
} // namespace bf

#endif // __BF_ACTION_ROTATE_SELECTION_HPP__
