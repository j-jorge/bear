/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
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
    /**
     * \brief Constructor.
     * \param lvl The level in which we take the selection.
     * \param dx The distance of the copy on the x-axis.
     * \param dy The distance of the copy on the y-axis.
     * \param add Tell if the copy has to be added to the current selection.
     */
    action_copy_selection
    ( const gui_level& lvl, double dx, double dy, bool add );

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
    unsigned int m_layer;

  }; // class action_copy_selection
} // namespace bf

#endif // __BF_ACTION_COPY_SELECTION_HPP__
