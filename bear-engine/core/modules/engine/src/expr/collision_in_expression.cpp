/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::collision_in_expression class.
 * \author Julien Jorge
 */
#include "bear/engine/expr/collision_in_expression.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::collision_in_expression::collision_in_expression()
  : m_colliding_item(NULL), m_collision_info(NULL)
{

} // collision_in_expression::collision_in_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item A pointer on the pointer that will be updated with the colliding
 *        item.
 * \param info A pointer on the pointer that will be updated with the collision
 *        info.
 */
bear::engine::collision_in_expression::collision_in_expression
( const_item_pointer const* item, const_collision_info_pointer const* info )
  : m_colliding_item(item), m_collision_info(info)
{

} // collision_in_expression::collision_in_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is a collision data in this instance.
 */
bool bear::engine::collision_in_expression::is_valid() const
{
  return (m_colliding_item != NULL ) && (m_collision_info != NULL);
} // collision_in_expression::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a pointer on the colliding item.
 */
const bear::engine::base_item*
bear::engine::collision_in_expression::get_item_ptr() const
{
  return *m_colliding_item;
} // collision_in_expression::get_item_ptr()

/*----------------------------------------------------------------------------*/
/**
 * \brief Member by pointer. Returns a pointer on the colliding item.
 */
const bear::engine::base_item*
bear::engine::collision_in_expression::operator->() const
{
  return get_item_ptr();
} // collision_in_expression::operator->()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indirection. Returns the colliding item.
 */
const bear::engine::base_item&
bear::engine::collision_in_expression::operator*() const
{
  return **m_colliding_item;
} // collision_in_expression::operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality. Compares the data in collision.
 * \param that The pointer to compare to.
 */
bool
bear::engine::collision_in_expression::operator==( base_item const* that ) const
{
  return *m_colliding_item == that;
} // collision_in_expression::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disquality. Compares the data in collision.
 * \param that The pointer to compare to.
 */
bool
bear::engine::collision_in_expression::operator!=( base_item const* that ) const
{
  return !operator==(that);
} // collision_in_expression::operator!=()
