/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of copying the selected items at a new position.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_COPY_SELECTION_HPP__
#define __BF_ACTION_COPY_SELECTION_HPP__

#include "bf/item_selection.hpp"
#include "bf/history/action_group.hpp"

#include <vector>

namespace bf
{
  /**
   * \brief The action of copying the selected items at a new position.
   * \author Julien Jorge
   */
  class action_copy_selection:
    public action_group
  {
  public:
    static action_group* create_for_layers
    ( const gui_level& lvl, std::vector<std::size_t> layers, double dx,
      double dy, bool add );

    action_copy_selection
    ( const gui_level& lvl, std::size_t layer_index, double dx, double dy,
      bool add );

    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    wxString get_description() const;

  private:
    /** \brief The new items. */
    item_selection m_new_items;

    /** \brief The previous selection. */
    item_selection m_previous_items;

    /** \brief Tell if the new items are added to the current selection or if
        they replace it. */
    const bool m_add_to_selection;

    /** \brief The index of the layer in which the items are added. */
    const std::size_t m_layer;

  }; // class action_copy_selection
} // namespace bf

#endif // __BF_ACTION_COPY_SELECTION_HPP__
