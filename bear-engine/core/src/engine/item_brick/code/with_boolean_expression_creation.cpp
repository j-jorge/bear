/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::with_boolean_expression_creation
 *        class.
 * \author Julien Jorge
 */
#include "engine/item_brick/with_boolean_expression_creation.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::
engine::with_boolean_expression_creation::~with_boolean_expression_creation()
{
  // nothing to do
} // with_boolean_expression_creation::~with_boolean_expression_creation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the expression.
 */
bear::expr::boolean_expression
bear::engine::with_boolean_expression_creation::get_expression() const
{
  return do_get_expression();
} // with_boolean_expression_creation::get_expression()
