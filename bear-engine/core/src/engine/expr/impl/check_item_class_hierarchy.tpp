/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::check_item_class_hierarchy class.
 * \author Julien Jorge.
 */

#include "engine/base_item.hpp"

#include <typeinfo>

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the data on the colliding item.
 * \param data The data on the colliding item.
 */
template<typename T>
void bear::engine::check_item_class_hierarchy<T>::set_collision_data
( const collision_in_expression& data )
{
  m_collision = data;
} // check_item_class_hierarchy::set_collision_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the data on the colliding item.
 */
template<typename T>
const bear::engine::collision_in_expression&
bear::engine::check_item_class_hierarchy<T>::get_collision_data() const
{
  return m_collision;
} // check_item_class_hierarchy::get_collision_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
template<typename T>
bear::expr::base_boolean_expression*
bear::engine::check_item_class_hierarchy<T>::clone() const
{
  return new check_item_class_hierarchy(*this);
} // check_item_class_hierarchy::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
template<typename T>
bool bear::engine::check_item_class_hierarchy<T>::evaluate() const
{
  return dynamic_cast<const T*>(m_collision.get_item_ptr()) != NULL;
} // check_item_class_hierarchy::evaluate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a formatted and human readable representation of this expression.
 */
template<typename T>
std::string
bear::engine::check_item_class_hierarchy<T>::formatted_string() const
{
  return std::string( "check_item_class_hierarchy( " )
    + typeid(T).name() + " )";
} // check_item_class_hierarchy::formatted_string()
