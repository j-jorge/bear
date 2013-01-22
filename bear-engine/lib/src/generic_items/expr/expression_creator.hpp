/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Template class to create items that can be use as a binary expression.
 * \author Julien Jorge
 */
#ifndef __BEAR_EXPRESSION_CREATOR_HPP__
#define __BEAR_EXPRESSION_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "expr/binary_boolean_expression.hpp"
#include "expr/binary_linear_expression.hpp"
#include "engine/item_brick/with_expression_creation.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  template<typename Result, typename LeftOperand, typename RightOperand>
  class binary_expression_creator;

  /**
   * \brief Template class to create items that can be use as a binary
   *        expression.
   *
   * This item is a parameter for other items. He kills himself at the end of
   * his build() method.
   *
   * The fields of this item are
   *  - \a left_operand: (item) \c the left operand in the expression. Must
   *    inherit from Expression::super,
   *  - \a right_operand: (item) \c the right operand in the expression. Must
   *    inherit from Expression::super.
   *
   * \author Julien Jorge
   */
  template<typename Result, typename LeftOperand, typename RightOperand>
  class binary_expression_creator:
    public engine::base_item,
    public engine::with_expression_creation<Result>::creation_class_type
  {
  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

    /** \brief The type of the class to use to instanciate the left operand. */
    typedef
    typename engine::with_expression_creation<LeftOperand>::creation_class_type
    left_creation_class_type;

    /** \brief The type of the class to use to instanciate the right operand. */
    typedef
    typename engine::with_expression_creation<RightOperand>::creation_class_type
    right_creation_class_type;

    /** \brief The type of the class to use to instanciate the expression. */
    typedef
    typename engine::with_expression_creation<Result>::creation_class_type
    creation_class_type;

    /** \brief The type of the expression created. */
    typedef typename creation_class_type::expression_type expression_type;

  public:
    bool set_item_field( const std::string& name, engine::base_item* value );

    void build();

  private:
    virtual expression_type do_get_expression() const;

  private:
    /** \brief The expression created by this item. */
    Result m_expr;

  }; // class binary_expression_creator

  /**
   * \brief This class is a boolean expression of type "boolean_equality".
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT boolean_equality_creator:
    public binary_expression_creator
  <expr::boolean_equality, expr::boolean_expression, expr::boolean_expression>
  {
    DECLARE_BASE_ITEM(boolean_equality_creator);
  }; // class boolean_equality_creator

  /**
   * \brief This class is a boolean expression of type "boolean_disequality".
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT boolean_disequality_creator:
    public binary_expression_creator
  < expr::boolean_disequality, expr::boolean_expression,
    expr::boolean_expression >
  {
    DECLARE_BASE_ITEM(boolean_disequality_creator);
  }; // class boolean_disequality_creator

  /**
   * \brief This class is a boolean expression of type "logical_and".
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT logical_and_creator:
    public binary_expression_creator
  <expr::logical_and, expr::boolean_expression, expr::boolean_expression>
  {
    DECLARE_BASE_ITEM(logical_and_creator);
  }; // class logical_and_creator

  /**
   * \brief This class is a boolean expression of type "logical_or".
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT logical_or_creator:
    public binary_expression_creator
  <expr::logical_or, expr::boolean_expression, expr::boolean_expression>
  {
    DECLARE_BASE_ITEM(logical_or_creator);
  }; // class logical_or_creator

  /**
   * \brief This class is a linear expression of type "equality".
   * \author Sébastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT linear_equality_creator:
    public binary_expression_creator
  <expr::linear_equality, expr::linear_expression, expr::linear_expression>
  {
    DECLARE_BASE_ITEM(linear_equality_creator);
  }; // class linear_equality_creator

  /**
   * \brief This class is a linear expression of type "plus".
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT linear_plus_creator:
    public binary_expression_creator
  <expr::linear_plus, expr::linear_expression, expr::linear_expression>
  {
    DECLARE_BASE_ITEM(linear_plus_creator);
  }; // class linear_plus_creator

  /**
   * \brief This class is a linear expression of type "minus".
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT linear_minus_creator:
    public binary_expression_creator
  <expr::linear_minus, expr::linear_expression, expr::linear_expression>
  {
    DECLARE_BASE_ITEM(linear_minus_creator);
  }; // class linear_minus_creator

  /**
   * \brief This class is a linear expression of type "multiplies".
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT linear_multiplies_creator:
    public binary_expression_creator
  <expr::linear_multiplies, expr::linear_expression, expr::linear_expression>
  {
    DECLARE_BASE_ITEM(linear_multiplies_creator);
  }; // class linear_multiplies_creator

  /**
   * \brief This class is a linear expression of type "divides".
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT linear_divides_creator:
    public binary_expression_creator
  <expr::linear_divides, expr::linear_expression, expr::linear_expression>
  {
    DECLARE_BASE_ITEM(linear_divides_creator);
  }; // class linear_divides_creator

} // namespace bear

#include "generic_items/expr/impl/expression_creator.tpp"

#endif // __BEAR_EXPRESSION_CREATOR_HPP__
