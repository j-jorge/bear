/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the methods for the bear::engine::item_flag_type
 *        type.
 * \author Julien Jorge
 */
#include "bear/engine/item_flag_type.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Operator "&" for item_flag_type.
 * \param a Left operand.
 * \param b Right operand.
 */
bear::engine::item_flag_type
bear::engine::operator&
(bear::engine::item_flag_type a, bear::engine::item_flag_type b)
{
  return item_flag_type(static_cast<int>(a) & static_cast<int>(b));
} // operator&() [item_flag_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Operator "|" for item_flag_type.
 * \param a Left operand.
 * \param b Right operand.
 */
bear::engine::item_flag_type
bear::engine::operator|
(bear::engine::item_flag_type a, bear::engine::item_flag_type b)
{
  return item_flag_type(static_cast<int>(a) | static_cast<int>(b));
} // operator|() [item_flag_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Operator "^" for item_flag_type.
 * \param a Left operand.
 * \param b Right operand.
 */
bear::engine::item_flag_type
bear::engine::operator^
(bear::engine::item_flag_type a, bear::engine::item_flag_type b)
{
  return item_flag_type(static_cast<int>(a) ^ static_cast<int>(b));
} // operator^() [item_flag_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Operator "~" for item_flag_type.
 * \param a Operand.
 */
bear::engine::item_flag_type
bear::engine::operator~(bear::engine::item_flag_type a)
{
  return item_flag_type(~static_cast<int>(a));
} // operator~() [item_flag_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Operator "&=" for item_flag_type.
 * \param a Left operand.
 * \param b Right operand.
 */
bear::engine::item_flag_type&
bear::engine::operator&=
(bear::engine::item_flag_type& a, bear::engine::item_flag_type b)
{
  return a = a & b;
} // operator&=() [item_flag_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Operator "|=" for item_flag_type.
 * \param a Left operand.
 * \param b Right operand.
 */
bear::engine::item_flag_type&
bear::engine::operator|=
(bear::engine::item_flag_type& a, bear::engine::item_flag_type b)
{
  return a = a | b;
} // operator|=() [item_flag_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Operator "^=" for item_flag_type.
 * \param a Left operand.
 * \param b Right operand.
 */
bear::engine::item_flag_type&
bear::engine::operator^=
(bear::engine::item_flag_type& a, bear::engine::item_flag_type b)
{
  return a = a ^ b;
} // operator^=() [item_flag_type]
