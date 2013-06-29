/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of removing an item in a layer.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_REMOVE_ITEM_HPP__
#define __BF_ACTION_REMOVE_ITEM_HPP__

#include "bf/history/action_group.hpp"

namespace bf
{
  class item_instance;
  class item_class;
  class type_field;

  /**
   * \brief The action of removing an item in a layer.
   * \author Julien Jorge
   */
  class action_remove_item:
    public level_action
  {
  public:
    action_remove_item( item_instance* item, unsigned int lay );

    /** \brief Destructor. */
    ~action_remove_item();

    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    bool is_identity( const gui_level& gui ) const;
    wxString get_description() const;

  private:
    void create_related_actions( gui_level& lvl );
    void create_related_actions_with( item_instance* item );
    void
    create_related_actions_with( item_instance* item, const item_class& c );

    void create_action_set_item_field_for
    ( item_instance* item, const type_field& f );
    void create_action_set_item_field_list_for
    ( item_instance* item, const type_field& f );

  private:
    /** \brief The item to remove. */
    item_instance* m_item;

    /** \brief The layer from which the item is removed. */
    unsigned int m_layer;

    /** \brief Tell if the item was selected. */
    bool m_selected;

    /** \brief Tell if the item was the main selection. */
    bool m_main_selection;

    /** \brief Tell if the action has been done or not. */
    bool m_done;

    /** \brief All actions happening before the removal of the item. */
    action_group* m_pre_actions;

    /** \brief All actions happening consequently to the removal of the item. */
    action_group* m_post_actions;

  }; // class action_remove_item
} // namespace bf

#endif // __BF_ACTION_REMOVE_ITEM_HPP__
