/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::timer_value class.
 * \author Julien Jorge
 */
#include "generic_items/expr/timer_value.hpp"

#include "generic_items/timer.hpp"

#include "expr/linear_function.hpp"

BASE_ITEM_EXPORT(timer_value, bear)

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::timer_value::timer_value()
: m_initial(false)
{

} // timer_value::timer_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the item is well initialized.
 */
bool bear::timer_value::is_valid() const
{
  return (m_timer != (timer*)NULL) && super::is_valid();
} // timer_value::()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of a field of type boolean.
 * \param name The name of the field to set.
 * \param value The value of the field.
 */
bool bear::timer_value::set_bool_field( const std::string& name, bool value )
{
  bool result(true);

  if ( name == "timer_value.initial" )
    m_initial = value;
  else
    result = super::set_bool_field(name, value);

  return result;
} // timer_value::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of a field of type item.
 * \param name The name of the field to set.
 * \param value The value of the field.
 */
bool bear::timer_value::set_item_field
( const std::string& name, engine::base_item* value )
{
  bool result(true);

  if ( name == "timer_value.timer" )
    m_timer = value;
  else
    result = super::set_item_field(name, value);

  return result;
} // timer_value::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the expression created by this item.
 */
bear::expr::linear_expression bear::timer_value::do_get_expression() const
{
  if ( m_initial )
    return expr::linear_function_maker
      ( m_timer, std::mem_fun_ref(&timer::get_initial_time) );
  else
    return expr::linear_function_maker
      ( m_timer, std::mem_fun_ref(&timer::get_time) );
} // timer_value::do_get_expression()
