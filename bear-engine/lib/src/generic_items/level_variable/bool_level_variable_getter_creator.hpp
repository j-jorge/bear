/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A boolean expression that returns the value of a level variable.
 * \author Sébastien Angibaud
 */
#ifndef __BEAR_BOOL_LEVEL_VARIABLE_GETTER_CREATOR_HPP__
#define __BEAR_BOOL_LEVEL_VARIABLE_GETTER_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "engine/function/bool_level_variable_getter.hpp"
#include "engine/item_brick/with_boolean_expression_creation.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A boolean expression that returns the value of a level variable.
   *
   * The valid fields for this item are
   *  - name: The name of the variable to get,
   *  - default_value: The default value of the variable, if not set,
   *  - any field supported by the parent classes.
   *
   * \author Sébastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT bool_level_variable_getter_creator:
    public engine::base_item,
    public engine::with_boolean_expression_creation
  {
    DECLARE_BASE_ITEM(bool_level_variable_getter_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    bool_level_variable_getter_creator();

    void build();
    bool is_valid() const;

    bool set_string_field
    ( const std::string& name, const std::string& value );
    bool set_bool_field( const std::string& name, bool value );

  private:
    virtual expr::boolean_expression do_get_expression() const;

  private:
    /** \brief The expression created by this item. */
    engine::bool_level_variable_getter m_expr;
  }; // class bool_level_variable_getter_creator
} // namespace bear

#endif // __BEAR_BOOL_LEVEL_VARIABLE_GETTER_CREATOR_HPP__
