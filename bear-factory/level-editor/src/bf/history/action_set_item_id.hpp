/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of setting the identifier of an item.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_SET_ITEM_ID_HPP__
#define __BF_ACTION_SET_ITEM_ID_HPP__

#include "bf/history/level_action.hpp"

#include <string>
#include <vector>
#include <map>

namespace bf
{
  class item_instance;
  class action_group;

  /**
   * \brief The action of setting the identifier of an item.
   * \author Julien Jorge
   */
  class action_set_item_id:
    public level_action
  {
  public:
    /** \brief The type of the collection of items modified in this action. */
    typedef std::vector<item_instance*> item_collection;

  private:
    /**
     * \brief The type of the container associating the identifier of an item
     *        with the corresponding item.
     */
    typedef std::map<item_instance*, std::string> item_to_id_map;

    /**
     * \brief The type of the container associating an identifier with another
     *        identifier.
     */
    typedef std::map<std::string, std::string> id_to_id_map;

  public:
    action_set_item_id
    ( item_instance* item, std::string id );
    action_set_item_id( item_collection items, std::string id );

    ~action_set_item_id();

    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    bool is_identity( const gui_level& gui ) const;
    wxString get_description() const;

    static wxString get_action_description();

  private:
    void drop_item_identifiers();

    void clear_identifiers( gui_level& lvl );

    void create_clear_field_actions( item_instance& item ) const;
    void create_clear_field_actions
    ( item_instance& item, std::string field_name ) const;
    void create_clear_list_field_actions
    ( item_instance& item, std::string field_name ) const;

    void rename_items( gui_level& lvl ) const;
    void restore_identifiers( gui_level& lvl ) const;

    void rename_item_references( gui_level& lvl, id_to_id_map m ) const;

  private:
    /** \brief The item to modify. */
    item_collection m_item;

    /** \brief The identifier of the item. */
    const std::string m_id;

    /** \brief The index of the layer in which the item is. */
    unsigned int m_layer_index;

    /** \brief The previous identifiers of the items, to be restored in the
        undo. */
    item_to_id_map m_old_identifiers;

    /** \brief The actions that remove the references to the renamed items when
        the identifier is set to an empty string. */
    action_group* m_clear_references_action;

  }; // class action_set_item_id
} // namespace bf

#endif // __BF_ACTION_SET_ITEM_ID_HPP__
