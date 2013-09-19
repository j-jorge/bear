/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::check_item_instance class.
 * \author Julien Jorge.
 */
#include "engine/expr/check_item_instance.hpp"

#include "engine/base_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the instance that validates the expression.
 * \param item The instance.
 */
void bear::engine::check_item_instance::set_instance( handle_type item )
{
  m_instance = item;
} // check_item_instance::set_instance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the instance that validates the expression.
 */
bear::engine::check_item_instance::handle_type
bear::engine::check_item_instance::get_instance() const
{
  return m_instance;
} // check_item_instance::get_instance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the data on the colliding item.
 * \param data The data on the colliding item.
 */
void bear::engine::check_item_instance::set_collision_data
( const collision_in_expression& data )
{
  m_collision = data;
} // check_item_instance::set_collision_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the data on the colliding item.
 */
const bear::engine::collision_in_expression&
bear::engine::check_item_instance::get_collision_data() const
{
  return m_collision;
} // check_item_instance::get_collision_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
bear::expr::base_boolean_expression*
bear::engine::check_item_instance::clone() const
{
  return new check_item_instance(*this);
} // check_item_instance::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
bool bear::engine::check_item_instance::evaluate() const
{
  if ( (m_collision == NULL) || (m_instance == NULL) )
    return false;
  else
    return m_collision.get_item_ptr() == m_instance.get();
} // check_item_instance::evaluate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a formatted and human readable representation of this expression.
 */
std::string bear::engine::check_item_instance::formatted_string() const
{
  std::ostringstream result;

  result << "check_item_instance( " << m_instance.get() << " )";

  return result.str();
} // check_item_instance::formatted_string()
