/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::count_items_by_class_name class.
 * \author Julien Jorge.
 */
#include "bear/engine/expr/count_items_by_class_name.hpp"

#include "bear/engine/world.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::count_items_by_class_name::count_items_by_class_name()
{
  // nothing to do
} // count_items_by_class_name::count_items_by_class_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param w An item in the world in which the items are searched.
 * \param c The name of the class of the counted items.
 */
bear::engine::count_items_by_class_name::count_items_by_class_name
( const base_item& w, const std::string& c )
  : m_world_proxy(w), m_class_name(c)
{

} // count_items_by_class_name::count_items_by_class_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the item through which we get the world in which the items are
 *        searched.
 * \param w The item.
 */
void bear::engine::count_items_by_class_name::set_world_through
( const base_item& w )
{
  m_world_proxy = &w;
} // count_items_by_class_name::set_world()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the class of the counted items.
 * \param c The name of the class.
 */
void
bear::engine::count_items_by_class_name::set_class_name( const std::string& c )
{
  m_class_name = c;
} // count_items_by_class_name::set_class_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
bear::expr::base_linear_expression*
bear::engine::count_items_by_class_name::clone() const
{
  return new count_items_by_class_name(*this);
} // count_items_by_class_name::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
double bear::engine::count_items_by_class_name::evaluate() const
{
  std::size_t result(0);

  if ( m_world_proxy == (base_item*)NULL )
    {
      claw::logger << claw::log_warning
                   << "count_items_by_class_name: the item is NULL, the "
        "evaluation is zero." << std::endl;

      return result;
    }

  world::const_item_iterator it;
  const world& w( m_world_proxy->get_world() );

  for ( it=w.living_items_begin(); it!=w.living_items_end(); ++it )
    if ( it->get_class_name() == m_class_name )
      ++result;

  return result;
} // count_items_by_class_name::evaluate()
