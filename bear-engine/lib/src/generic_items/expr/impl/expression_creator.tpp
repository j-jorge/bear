/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::expression_creator class.
 * \author Julien Jorge
 */

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "item".
 * \param name The name of the field.
 * \param value The value of the field.
 */
template<typename Result, typename LeftOperand, typename RightOperand>
bool bear::binary_expression_creator
<Result, LeftOperand, RightOperand>::set_item_field
( const std::string& name, engine::base_item* value )
{
  bool result = true;

  if ( name == "binary_expression_creator.left_operand" )
    {
      left_creation_class_type* e
        ( dynamic_cast<left_creation_class_type*>(value) );

      if ( e != NULL )
        m_expr.set_left_operand(e->get_expression());
      else
        claw::logger << claw::log_error << name
                     << ": item is not of a valid type." << std::endl;
    }
  else if ( name == "binary_expression_creator.right_operand" )
    {
      right_creation_class_type* e
        ( dynamic_cast<right_creation_class_type*>(value) );

      if ( e != NULL )
        m_expr.set_right_operand(e->get_expression());
      else
        claw::logger << claw::log_error << name
                     << ": item is not of a valid type." << std::endl;
    }
  else
    result = super::set_item_field(name, value);

  return result;
} // binary_expression_creator::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
template<typename Result, typename LeftOperand, typename RightOperand>
void bear::binary_expression_creator<Result, LeftOperand, RightOperand>::build()
{
  kill();
} // binary_expression_creator::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the expression created by this item.
 */
template<typename Result, typename LeftOperand, typename RightOperand>
typename bear::binary_expression_creator
<Result, LeftOperand, RightOperand>::expression_type
bear::binary_expression_creator
<Result, LeftOperand, RightOperand>::do_get_expression() const
{
  return m_expr;
} // binary_expression_creator::do_get_expression()
