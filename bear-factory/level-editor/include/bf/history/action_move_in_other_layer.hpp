/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of moving selection in an other layer.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_MOVE_IN_OTHER_LAYER_HPP__
#define __BF_ACTION_MOVE_IN_OTHER_LAYER_HPP__

#include "bf/history/action_group.hpp"
#include "bf/item_selection.hpp"

namespace bf
{
  /**
   * \brief The action of moving selection in a other layer.
   * \author Sébastien Angibaud
   */
  class action_move_in_other_layer:
    public action_group
  {
  public:
    /**
     * \brief Constructor.
     * \param lvl The level in which we take the selection.
     * \param new_layer The layer in which the items are added.
     */
    action_move_in_other_layer( const gui_level& lvl, unsigned int new_layer );

    wxString get_description() const;
    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

  private:
    /** \brief The new items. */
    item_selection m_new_items;

    /** \brief The previous selection. */
    item_selection m_previous_items;

    /** \brief The previous selection in the layer containing picked item. */
    item_selection m_previous_in_other_layer_items;

    /** \brief The index of the layer in which the items are added. */
    unsigned int m_new_layer;

    /** \brief The index of the layer in which the items are picked. */
    unsigned int m_last_layer;
  }; // class action_move_in_other_layer
} // namespace bf

#endif // __BF_ACTION_MOVE_IN_OTHER_LAYER_HPP__
