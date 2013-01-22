/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of changing the value of a field of an item.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_SET_ITEM_FIELD_HPP__
#define __BF_ACTION_SET_ITEM_FIELD_HPP__

#include "bf/history/level_action.hpp"
#include <string>

namespace bf
{
  class item_instance;

  /**
   * \brief The action of changing the value of a field of an item.
   * \author Julien Jorge
   */
  template<typename ValueType>
  class action_set_item_field:
    public level_action
  {
  public:
    /**
     * \brief Constructor. Remove the value of a field.
     * \param item The item to modify.
     * \param name The name of the field to change.
     */
    action_set_item_field( item_instance* item, const std::string& name );

    /**
     * \brief Constructor. Give a new value to a field.
     * \param item The item to modify.
     * \param name The name of the field to change.
     * \param val The new value of the field.
     */
    action_set_item_field
    ( item_instance* item, const std::string& name, const ValueType& val );

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

    /** \brief The name of the field to set. */
    const std::string m_name;

    /** \brief The new value of the field. */
    ValueType m_value;

    /** \brief Tell if a value has been given to m_value. */
    bool m_has_value;

  }; // class action_set_item_field
} // namespace bf

#include "bf/history/impl/action_set_item_field.tpp"

#endif // __BF_ACTION_SET_ITEM_FIELD_HPP__
