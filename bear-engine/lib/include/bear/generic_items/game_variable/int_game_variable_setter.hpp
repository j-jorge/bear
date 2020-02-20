/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item that sets the value of an integer game variable.
 * \author Angibaud Sebastien
 */
#ifndef __BEAR_INT_GAME_VARIABLE_SETTER_HPP__
#define __BEAR_INT_GAME_VARIABLE_SETTER_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/engine/item_brick/make_autokill_from_class.hpp"
#include "bear/engine/item_brick/make_toggle_on_from_class.hpp"

#include "bear/engine/export.hpp"
#include "bear/generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A item that sets the value of an integer game variable.
   *
   * The valid fields for this item are
   *  - name The name of the variable to set.
   *  - value The new value of the variable.
   *  - any field supported by the parent classes.
   *
   * \author Sebastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT int_game_variable_setter:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(int_game_variable_setter);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    int_game_variable_setter();

    bool is_valid() const;
    bool set_integer_field( const std::string& name, int value );
    bool set_string_field
    ( const std::string& name, const std::string& value );

    void assign_game_variable_value() const;

  private:
    /** \brief The name of the variable. */
    std::string m_name;

    /** \brief The new value of the variable. */
    int m_value;
  }; // class int_game_variable_setter

  /** \brief A game variable setter that sets the variable in its build and
      kills itself. */
  class GENERIC_ITEMS_EXPORT int_game_variable_setter_suicide:
    public engine::make_autokill_from_class_const
    < int_game_variable_setter,
      &int_game_variable_setter::assign_game_variable_value >
  {
    DECLARE_BASE_ITEM(int_game_variable_setter_suicide);
  }; // class int_game_variable_setter_suicide

  /** \brief A game variable setter in a toggle. The value of the variable is
      set when toggled on. */
  class GENERIC_ITEMS_EXPORT int_game_variable_setter_toggle:
    public engine::make_toggle_on_from_class_const
    < int_game_variable_setter,
      &int_game_variable_setter::assign_game_variable_value >
  {
    DECLARE_BASE_ITEM(int_game_variable_setter_toggle);
  }; // class int_game_variable_setter_toggle

} // namespace bear

#endif // __BEAR_INT_GAME_VARIABLE_SETTER_HPP__
