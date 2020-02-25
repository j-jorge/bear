/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of adding an item in a layer.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_ADD_ITEM_HPP__
#define __BF_ACTION_ADD_ITEM_HPP__

#include "bf/history/level_action.hpp"

namespace bf
{
  class item_instance;

  /**
   * \brief The action of adding an item in a layer.
   * \author Julien Jorge
   */
  class action_add_item:
    public level_action
  {
  public:
    /**
     * \brief Constructor.
     * \param item The item to add.
     * \param lay The layer in which the item is added.
     */
    action_add_item( item_instance* item, unsigned int lay );

    /** \brief Destructor. */
    ~action_add_item();

    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    bool is_identity( const gui_level& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The item to add. */
    item_instance* const m_item;

    /** \brief The layer in which the item is added. */
    const unsigned int m_layer;

    /** \brief Tell if the action has been done or not. */
    bool m_done;

  }; // class action_add_item
} // namespace bf

#endif // __BF_ACTION_ADD_ITEM_HPP__
