/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Template class to create items that can be use as a binary expression.
 * \author Julien Jorge
 */
#ifndef __BEAR_LOGICAL_NOT_CREATOR_HPP__
#define __BEAR_LOGICAL_NOT_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/with_boolean_expression_creation.hpp"
#include "expr/logical_not.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class is a boolean expression of type "logical_not".
   *
   * This item is a parameter for other items. He kills himself at the end of
   * his build() method.
   *
   * The fields of this item are
   *  - \a operand: (item) \c the operand of the operator. Must inherit from
   *     with_boolean_expression_creation.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT logical_not_creator:
    public engine::base_item,
    public engine::with_boolean_expression_creation
  {
    DECLARE_BASE_ITEM(logical_not_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    bool set_item_field( const std::string& name, engine::base_item* value );

    void build();

  private:
    virtual expr::boolean_expression do_get_expression() const;

  private:
    /** \brief The resulting expression. */
    expr::logical_not m_expr;

  }; // class logical_not_creator

} // namespace bear

#endif // __BEAR_LOGICAL_NOT_CREATOR_HPP__
