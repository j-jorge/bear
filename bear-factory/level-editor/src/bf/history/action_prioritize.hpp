/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of setting the compilation priority on an item.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_PRIORITIZE_HPP__
#define __BF_ACTION_PRIORITIZE_HPP__

#include "bf/history/level_action.hpp"

namespace bf
{
  class item_instance;

  /**
   * \brief The action of setting the compilation priority on an item.
   * \author Julien Jorge
   */
  class action_prioritize:
    public level_action
  {
  public:
    action_prioritize( item_instance* item, unsigned int lay );

    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    bool is_identity( const gui_level& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The item to remove. */
    item_instance* m_item;

    /** \brief The layer from which the item is removed. */
    unsigned int m_layer;

  }; // class action_prioritize
} // namespace bf

#endif // __BF_ACTION_PRIORITIZE_HPP__
