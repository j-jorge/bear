/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class gives a boolean expression to one engine::with_trigger or
 *        more, then die.
 * \author Julien Jorge
 */
#ifndef __BEAR_APPLIED_EXPRESSION_HPP__
#define __BEAR_APPLIED_EXPRESSION_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/engine/item_brick/with_expression_assignment.hpp"
#include "bear/engine/item_brick/with_expression_creation.hpp"

#include "bear/generic_items/class_export.hpp"
#include "bear/engine/export.hpp"

namespace bear
{
  /**
   * \brief This class gives a boolean expression to one engine::with_trigger or
   *        more, then die.
   *
   * The fields of this item are
   *  - \a expression: (item) \b [required] The expression. Must inherit from
   *    engine::with_expression_creation,
   *  - \a trigger: (item) \b [required] The triggers to which the condition
   *    is applied. Must inherit from engine::with_trigger.
   *
   * \author Julien Jorge
   */
  template<typename Expression>
  class applied_expression:
    public engine::base_item
  {
  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  private:
    /** \brief The base class of the expression. */
    typedef typename Expression::base_expression_type base_expression_type;

    /** \brief The type of the instances receiving the expression. */
    typedef typename engine::with_expression_assignment
    <
      base_expression_type
    >::assignment_class_type
    assignment_class_type;

    /** \brief The type of the instances from which we get the expression. */
    typedef typename engine::with_expression_creation
    <
      base_expression_type
    >::creation_class_type
    creation_class_type;

  public:
    bool set_item_field( const std::string& name, engine::base_item* value );
    bool set_item_list_field
    ( const std::string& name, const std::vector<engine::base_item*>& value );

    bool is_valid() const;

    void build();

  private:
    /** \brief The items receiving the expression. */
    std::vector<assignment_class_type*> m_items;

    /** \brief The expression. */
    Expression m_expression;

  }; // class applied_expression

  /** \brief Applied expression for boolean expressions. */
  class GENERIC_ITEMS_EXPORT applied_boolean_expression:
    public applied_expression<expr::boolean_expression>
  {
    DECLARE_BASE_ITEM(applied_boolean_expression);
  }; // class applied_boolean_expression

  /** \brief Applied expression for linear expressions. */
  class GENERIC_ITEMS_EXPORT applied_linear_expression:
    public applied_expression<expr::linear_expression>
  {
    DECLARE_BASE_ITEM(applied_linear_expression);
  }; // class applied_linear_expression

} // namespace bear

#include "bear/generic_items/expr/impl/applied_expression.tpp"

#endif // __BEAR_APPLIED_EXPRESSION_HPP__
