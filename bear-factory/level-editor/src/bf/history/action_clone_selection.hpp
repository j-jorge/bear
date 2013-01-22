/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of cloning the selected items.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_CLONE_SELECTION_HPP__
#define __BF_ACTION_CLONE_SELECTION_HPP__

#include "bf/item_selection.hpp"
#include "bf/history/action_group.hpp"

namespace bf
{
  class item_instance;

  /**
   * \brief The action of deleting the selected items.
   * \author Julien Jorge
   */
  class action_clone_selection:
    public action_group
  {
  public:
    /**
     * \brief Constructor.
     * \param lvl The level in which we take the selection.
     * \param x_count Count of clones on the x-axis.
     * \param y_count Count of clones on the y-axis.
     * \param x_offset Offset on the x-axis.
     * \param y_offset Offset on the y-axis.
     * \param add Add the clones in the selection.
     */
    action_clone_selection
    ( const gui_level& lvl, unsigned int x_count, unsigned int y_count,
      double x_offset, double y_offset, bool add );

    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    wxString get_description() const;

  private:
    /**
     * \brief Clone an item.
     * \param item The item to clone.
     * \param x_count Count of clones on the x-axis.
     * \param y_count Count of clones on the y-axis.
     * \param x_offset Offset on the x-axis.
     * \param y_offset Offset on the y-axis.
     */
    void clone_item
    ( const item_instance& item, unsigned int x_count, unsigned int y_count,
      double x_offset, double y_offset );

  private:
    /** \brief The new items, if we have to add them in the selection. */
    item_selection m_new_items;

    /** \brief The index of the layer in which the items are added. */
    unsigned int m_layer;

  }; // class action_clone_selection
} // namespace bf

#endif // __BF_ACTION_CLONE_SELECTION_HPP__
