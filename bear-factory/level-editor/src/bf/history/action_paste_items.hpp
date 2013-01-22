/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of pasteing the items from the clipboard.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_PASTE_ITEMS_HPP__
#define __BF_ACTION_PASTE_ITEMS_HPP__

#include "bf/item_selection.hpp"
#include "bf/history/action_group.hpp"

namespace bf
{
  struct item_copy;

  /**
   * \brief The action of pasting some items in a layer.
   * \author Julien Jorge
   */
  class action_paste_items:
    public action_group
  {
  public:
    /**
     * \brief Constructor.
     * \param lvl The level in which we take the selection.
     * \param ref The items to paste.
     * \param x The x-position of the main selection.
     * \param y The y-position of the main selection.
     */
    action_paste_items
    ( const gui_level& lvl, const item_copy& ref, double x,
      double y );

    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    wxString get_description() const;

  private:
    /** \brief The new items. */
    item_selection m_new_items;

    /** \brief The previous selection. */
    item_selection m_previous_items;

    /** \brief The index of the layer in which the items are added. */
    unsigned int m_layer;

  }; // class action_paste_items
} // namespace bf

#endif // __BF_ACTION_PASTE_ITEMS_HPP__
