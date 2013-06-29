/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A linear expression that returns the value of a game variable.
 * \author Julien Jorge
 */
#ifndef __BEAR_U_INT_GAME_VARIABLE_GETTER_CREATOR_HPP__
#define __BEAR_U_INT_GAME_VARIABLE_GETTER_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "engine/function/linear_game_variable_getter.hpp"
#include "engine/item_brick/with_linear_expression_creation.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A linear expression that returns the value of a game variable.
   *
   * The valid fields for this item are
   *  - name: The name of the variable to get,
   *  - default_value: The default value of the variable, if not set,
   *  - any field supported by the parent classes.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT u_int_game_variable_getter_creator:
    public engine::base_item,
    public engine::with_linear_expression_creation
  {
    DECLARE_BASE_ITEM(u_int_game_variable_getter_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    u_int_game_variable_getter_creator();

    void build();
    bool is_valid() const;

    bool set_string_field
    ( const std::string& name, const std::string& value );
    bool set_u_integer_field( const std::string& name, unsigned int value );

  private:
    virtual expr::linear_expression do_get_expression() const;

  private:
    /** \brief The expression created by this item. */
    engine::linear_game_variable_getter<unsigned int> m_expr;

  }; // class u_int_game_variable_getter_creator
} // namespace bear

#endif // __BEAR_U_INT_GAME_VARIABLE_GETTER_CREATOR_HPP__
