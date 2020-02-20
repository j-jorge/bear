/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of changing the position of the left edge of an item.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_SET_ITEM_LEFT_HPP__
#define __BF_ACTION_SET_ITEM_LEFT_HPP__

#include "bf/history/level_action.hpp"

namespace bf
{
  class item_instance;

  /**
   * \brief The action of changing the position of the left edge of an item.
   * \author Julien Jorge
   */
  class action_set_item_left:
    public level_action
  {
  public:
    /**
     * \brief Constructor.
     * \param item The item to modify.
     * \param p The new position of the left edge.
     */
    action_set_item_left( item_instance* item, double p );

    /**
     * \remark Calling execute() two times will restore the initial position.
     */
    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    bool is_identity( const gui_level& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The item to modify. */
    item_instance* m_item;

    /** \brief The position of the left edge. */
    double m_position;

  }; // class action_set_item_left
} // namespace bf

#endif // __BF_ACTION_SET_ITEM_LEFT_HPP__
