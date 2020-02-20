/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::with_trigger class.
 * \author Julien Jorge
 */
#include "bear/engine/item_brick/with_trigger.hpp"

#include "bear/engine/item_brick/with_toggle.hpp"
#include "bear/universe/physical_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::with_trigger::with_trigger()
  : m_colliding_item(NULL), m_collision_info(NULL)
{

} // with_trigger::with_trigger()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bear::engine::with_trigger::with_trigger( const with_trigger& that )
  :  with_boolean_expression_assignment(that),
     m_colliding_item(NULL), m_collision_info(NULL)
{
  // the collision_in_expression in the expression cannot be copied, so we just
  // not copy the expression.
} // with_trigger::with_trigger()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment.
 * \param that The instance to copy from.
 */
bear::engine::with_trigger
bear::engine::with_trigger::operator=( const with_trigger& that )
{
  // the collision_in_expression in the expression cannot be copied, so we just
  // not copy the expression.

  return *this;
} // with_trigger::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the condition to verify to turn on the toggles.
 */
void bear::engine::with_trigger::set_condition
( const expr::boolean_expression& e )
{
  m_condition = e;
} // with_trigger::set_condition()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the condition to verify to turn on the toggles.
 */
const bear::expr::boolean_expression&
bear::engine::with_trigger::get_condition() const
{
  return m_condition;
} // with_trigger::get_condition()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the informations on a collision that can be used in an expression.
 */
bear::engine::collision_in_expression
bear::engine::with_trigger::get_collision_in_expression() const
{
  return collision_in_expression(&m_colliding_item, &m_collision_info);
} // with_trigger::get_collision_in_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the informations on a collision that can be used in an expression.
 * \param that The colliding item.
 * \param info Some informations about the collision.
 */
void bear::engine::with_trigger::set_collision_data
( base_item& that, universe::collision_info& info )
{
  m_colliding_item = &that;
  m_collision_info = &info;
} // with_trigger::set_collision_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the informations on a collision that can be used in an expression.
 */
void bear::engine::with_trigger::clear_collision_data()
{
  m_colliding_item = NULL;
  m_collision_info = NULL;
} // with_trigger::clear_collision_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check the condition and call the adequate on_trigger_* method.
 * \param activator The item that activated the trigger.
 */
void bear::engine::with_trigger::check_condition( base_item* activator )
{
  if ( m_condition )
    on_trigger_on(activator);
  else
    on_trigger_off(activator);
} // with_trigger::check_condition()

/*----------------------------------------------------------------------------*/
/**
 * \brief Function called when the trigger is activated.
 * \param activator The item that activated the trigger.
 */
void bear::engine::with_trigger::on_trigger_on( base_item* activator )
{

} // with_trigger::on_trigger_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Function called when the trigger is deactivated.
 * \param activator The item that activated the trigger.
 */
void bear::engine::with_trigger::on_trigger_off( base_item* activator )
{

} // with_trigger::on_trigger_off()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the expression used for the condition.
 * \param e The expression.
 */
void bear::engine::with_trigger::do_set_expression
( const expr::boolean_expression& e )
{
  set_condition(e);
} // with_trigger::do_set_expression()

