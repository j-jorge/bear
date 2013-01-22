/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::check_item_class class.
 * \author Julien Jorge.
 */
#include "engine/expr/check_item_class.hpp"

#include "engine/base_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the class.
 * \param class_name The name of the class.
 */
void bear::engine::check_item_class::set_class_name
( const std::string& class_name )
{
  m_class_name = class_name;
} // check_item_class::set_class_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the class.
 */
const std::string& bear::engine::check_item_class::get_class_name() const
{
  return m_class_name;
} // check_item_class::get_class_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the data on the colliding item.
 * \param data The data on the colliding item.
 */
void bear::engine::check_item_class::set_collision_data
( const collision_in_expression& data )
{
  m_collision = data;
} // check_item_class::set_collision_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the data on the colliding item.
 */
const bear::engine::collision_in_expression&
bear::engine::check_item_class::get_collision_data() const
{
  return m_collision;
} // check_item_class::get_collision_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
bear::expr::base_boolean_expression*
bear::engine::check_item_class::clone() const
{
  return new check_item_class(*this);
} // check_item_class::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
bool bear::engine::check_item_class::evaluate() const
{
  if ( m_collision == NULL )
    return false;
  else
    return m_collision->get_class_name() == m_class_name;
} // check_item_class::evaluate()
