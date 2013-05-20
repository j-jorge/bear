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
  class item_selection;

  /**
   * \brief The action of rotating the selected items.
   * \author Sebastien Angibaud
   */
  class action_rotate_selection:
    public action_group
  {
  public:
    action_rotate_selection( const item_selection& selection, bool clockwise );

    wxString get_description() const;

  }; // class action_rotate_selection
} // namespace bf

#endif // __BF_ACTION_ROTATE_SELECTION_HPP__
