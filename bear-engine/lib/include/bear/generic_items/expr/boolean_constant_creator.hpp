/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Class to create an item that is a boolean constant.
 * \author S�bastien Angibaud
 */
#ifndef __BEAR_BOOLEAN_CONSTANT_CREATOR_HPP__
#define __BEAR_BOOLEAN_CONSTANT_CREATOR_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/expr/boolean_constant.hpp"
#include "bear/engine/item_brick/with_boolean_expression_creation.hpp"

#include "bear/engine/export.hpp"
#include "bear/generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class is a boolean expression of type "boolean_constant".
   *
   * This item is a parameter for other items. He kills himself at the end of
   * his build() method.
   *
   * The fields of this item are
   *  - \a operand: (boolean) \c the value of the constant.
   *
   * \author S�bastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT boolean_constant_creator:
    public engine::base_item,
    public engine::with_boolean_expression_creation
  {
    DECLARE_BASE_ITEM(boolean_constant_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    boolean_constant_creator();

    bool set_bool_field( const std::string& name, bool value );

    void build();

  private:
    virtual expr::boolean_expression do_get_expression() const;

  private:
    /** \brief The expression created by this item. */
    expr::boolean_constant m_expr;

  }; // class boolean_constant_creator

} // namespace bear

#endif // __BEAR_BOOLEAN_CONSTANT_CREATOR_HPP__
