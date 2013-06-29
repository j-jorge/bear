/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::logical_not_creator class.
 * \author Julien Jorge
 */
#include "generic_items/expr/logical_not_creator.hpp"

#include <claw/logger.hpp>

BASE_ITEM_EXPORT( logical_not_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "item".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::logical_not_creator::set_item_field
( const std::string& name, engine::base_item* value )
{
  bool result = true;

  if ( name == "logical_not_creator.operand" )
    {
      engine::with_boolean_expression_creation* e
        ( dynamic_cast<engine::with_boolean_expression_creation*>(value) );

      if ( e != NULL )
        m_expr.set_operand(e->get_expression());
      else
        claw::logger << claw::log_error << name << ": item is not of a type "
                     << "'with_boolean_expression_creation'." << std::endl;
    }
  else
    result = super::set_item_field(name, value);

  return result;
} // logical_not_creator::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::logical_not_creator::build()
{
  kill();
} // logical_not_creator::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the expression created by this item.
 */
bear::expr::boolean_expression
bear::logical_not_creator::do_get_expression() const
{
  return m_expr;
} // logical_not_creator::do_get_expression()
