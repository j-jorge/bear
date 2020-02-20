/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::with_linear_expression_creation
 *        class.
 * \author Julien Jorge
 */
#include "bear/engine/item_brick/with_linear_expression_creation.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::with_linear_expression_creation::~with_linear_expression_creation
()
{
  // nothing to do
} // with_linear_expression_creation::~with_linear_expression_creation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the expression.
 */
bear::expr::linear_expression
bear::engine::with_linear_expression_creation::get_expression() const
{
  return do_get_expression();
} // with_linear_expression_creation::get_expression()
