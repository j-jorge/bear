/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of removing the compilation priority on an item.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_DEPRIORITIZE_HPP__
#define __BF_ACTION_DEPRIORITIZE_HPP__

#include "bf/history/level_action.hpp"

namespace bf
{
  class item_instance;

  /**
   * \brief The action of removing the compilation priority on an item.
   * \author Julien Jorge
   */
  class action_deprioritize:
    public level_action
  {
  public:
    action_deprioritize( item_instance* item, unsigned int lay );

    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    bool is_identity( const gui_level& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The item to remove. */
    item_instance* m_item;

    /** \brief The layer from which the item is removed. */
    unsigned int m_layer;

    /** \brief The previous priority of the item. */
    int m_priority;

  }; // class action_deprioritize
} // namespace bf

#endif // __BF_ACTION_DEPRIORITIZE_HPP__
