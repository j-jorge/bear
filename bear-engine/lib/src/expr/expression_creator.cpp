/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the classes inheriting from
 *        bear::expression_creator.
 * \author Julien Jorge
 */
#include "bear/generic_items/expr/expression_creator.hpp"

BASE_ITEM_EXPORT( boolean_equality_creator, bear )
BASE_ITEM_EXPORT( boolean_disequality_creator, bear )
BASE_ITEM_EXPORT( logical_and_creator, bear )
BASE_ITEM_EXPORT( logical_or_creator, bear )

BASE_ITEM_EXPORT( linear_equality_creator, bear )
BASE_ITEM_EXPORT( linear_plus_creator, bear )
BASE_ITEM_EXPORT( linear_minus_creator, bear )
BASE_ITEM_EXPORT( linear_multiplies_creator, bear )
BASE_ITEM_EXPORT( linear_divides_creator, bear )
