/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of changing the class of an item.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_SET_ITEM_CLASS_HPP__
#define __BF_ACTION_SET_ITEM_CLASS_HPP__

#include "bf/history/level_action.hpp"

#include "bf/item_instance.hpp"

namespace bf
{
  class item_class;

  /**
   * \brief The action of changing the class of an item.
   * \author Julien Jorge
   */
  class action_set_item_class:
    public level_action
  {
  public:
    action_set_item_class( item_instance* item, item_class const* c );

    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    bool is_identity( const gui_level& gui ) const;
    wxString get_description() const;

    static wxString get_action_description();

  private:
    /** \brief The item to modify. */
    item_instance* const m_item;

    /** \brief The new class of the item. */
    item_class const* m_class;

    /** \brief The old item, before changing its class. */
    const item_instance m_backup;

  }; // class action_set_item_class
} // namespace bf

#endif // __BF_ACTION_SET_ITEM_CLASS_HPP__
