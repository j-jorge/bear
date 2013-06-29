/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::expr::binary_expression class.
 * \author Julien Jorge.
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
template<typename Base, typename Operand, typename Function>
bear::expr::binary_expression<Base, Operand, Function>::binary_expression()
{

} // binary_expression::binary_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy contructor.
 */
template<typename Base, typename Operand, typename Function>
bear::expr::binary_expression<Base, Operand, Function>::binary_expression
(const binary_expression<Base, Operand, Function>& that)
  : m_left(that.m_left), m_right(that.m_right)
{

} // binary_expression::binary_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 * \param left The left operand.
 * \param right The right operand.
 */
template<typename Base, typename Operand, typename Function>
bear::expr::binary_expression<Base, Operand, Function>::binary_expression
( const operand_type& left, const operand_type& right )
  : m_left(left), m_right(right)
{

} // binary_expression::binary_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the left operand.
 */
template<typename Base, typename Operand, typename Function>
const typename
bear::expr::binary_expression<Base, Operand, Function>::operand_type&
bear::expr::binary_expression<Base, Operand, Function>::get_left_operand() const
{
  return m_left;
} // binary_expression::get_left_operand()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the left operand.
 * \param op The operand.
 */
template<typename Base, typename Operand, typename Function>
void bear::expr::binary_expression<Base, Operand, Function>::set_left_operand
( const operand_type& op )
{
  m_left = op;
} // binary_expression::set_left_operand()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the right operand.
 */
template<typename Base, typename Operand, typename Function>
const typename
bear::expr::binary_expression<Base, Operand, Function>::operand_type&
bear::expr::binary_expression<Base, Operand, Function>::
get_right_operand() const
{
  return m_right;
} // binary_expression::get_right_operand()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the right operand.
 * \param op The operand.
 */
template<typename Base, typename Operand, typename Function>
void bear::expr::binary_expression<Base, Operand, Function>::set_right_operand
( const operand_type& op )
{
  m_right = op;
} // binary_expression::set_right_operand()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
template<typename Base, typename Operand, typename Function>
Base* bear::expr::binary_expression<Base, Operand, Function>::clone() const
{
  return new binary_expression<Base, Operand, Function>(*this);
} // binary_expression::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
template<typename Base, typename Operand, typename Function>
typename bear::expr::binary_expression<Base, Operand, Function>::result_type
bear::expr::binary_expression<Base, Operand, Function>::evaluate() const
{
  Function f;
  return f(get_left_operand().evaluate(), get_right_operand().evaluate());
} // binary_expression::evaluate()
