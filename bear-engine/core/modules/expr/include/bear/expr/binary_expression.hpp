/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An operation on two expressions.
 * \author Julien Jorge
 */
#ifndef __EXPR_BINARY_EXPRESSION_HPP__
#define __EXPR_BINARY_EXPRESSION_HPP__

namespace bear
{
  namespace expr
  {
    /**
     * \brief A linear expression with two operands.
     * \author Julien Jorge
     */
    template<typename Base, typename Operand, typename Function>
    class binary_expression:
      public Base
    {
    public:
      /** \brief The type of the result. */
      typedef typename Base::result_type result_type;

      /** \brief The type of the operands. */
      typedef Operand operand_type;

      /** \brief The type of this class. */
      typedef binary_expression<Base, Operand, Function> self_type;

    public:
      binary_expression();
      binary_expression(const self_type& that);
      binary_expression
      ( const operand_type& left, const operand_type& right );

      const operand_type& get_left_operand() const;
      void set_left_operand( const operand_type& op );

      const operand_type& get_right_operand() const;
      void set_right_operand( const operand_type& op );

      Base* clone() const;
      result_type evaluate() const;

      std::string formatted_string() const;

    private:
      /** \brief The left operand. */
      operand_type m_left;

      /** \brief The right operand. */
      operand_type m_right;

    }; // class binary_expression

  } // namespace expr
} // namespace bear

#include "bear/expr/impl/binary_expression.tpp"

#endif // __EXPR_BINARY_EXPRESSION_HPP__
