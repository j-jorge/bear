/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::with_boolean_expression_assignment
 *        class.
 * \author Julien Jorge
 */
#include "engine/item_brick/with_boolean_expression_assignment.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::with_boolean_expression_assignment::
~with_boolean_expression_assignment
()
{
  // nothing to do
} // with_boolean_expression_assignment::~with_boolean_expression_assignment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the expression.
 * \param e The expression to assign to the item.
 */
void bear::engine::with_boolean_expression_assignment::set_expression
( const expr::boolean_expression& e )
{
  do_set_expression(e);
} // with_boolean_expression_assignment::set_expression()
