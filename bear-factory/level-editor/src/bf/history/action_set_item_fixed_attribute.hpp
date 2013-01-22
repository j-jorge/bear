/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of changing the "fixed" attribute of an item.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_SET_ITEM_FIXED_ATTRIBUTE_HPP__
#define __BF_ACTION_SET_ITEM_FIXED_ATTRIBUTE_HPP__

#include "bf/history/level_action.hpp"

namespace bf
{
  class item_instance;

  /**
   * \brief The action of changing the "fixed" attribute of an item.
   * \author Julien Jorge
   */
  class action_set_item_fixed_attribute:
    public level_action
  {
  public:
    /**
     * \brief Constructor.
     * \param item The item to modify.
     * \param fixed Tell if we fix the item or not.
     */
    action_set_item_fixed_attribute( item_instance* item, bool fixed );

    /**
     * \remark Calling execute() two times will restore the initial value.
     */
    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    bool is_identity( const gui_level& gui ) const;
    wxString get_description() const;

    static wxString get_action_description();

  private:
    /** \brief The item to modify. */
    item_instance* const m_item;

    /** \brief Tell if we fix the item or not. */
    bool m_fixed;

  }; // class action_set_item_fixed_attribute
} // namespace bf

#endif // __BF_ACTION_SET_ITEM_FIXED_ATTRIBUTE_HPP__
