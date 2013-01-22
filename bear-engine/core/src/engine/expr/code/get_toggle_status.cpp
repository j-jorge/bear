/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::get_toggle_status class.
 * \author Julien Jorge.
 */
#include "engine/expr/get_toggle_status.hpp"

#include "engine/base_item.hpp"
#include "engine/item_brick/with_toggle.hpp"
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::get_toggle_status::get_toggle_status()
{
  // nothing to do
} // get_toggle_status::get_toggle_status()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The toggle from which we take the status.
 */
bear::engine::get_toggle_status::get_toggle_status( const base_item& item )
  : m_toggle(&item)
{

} // get_toggle_status::get_toggle_status()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the toggle from which we take the status.
 * \param item The toggle.
 */
void bear::engine::get_toggle_status::set_toggle( const base_item& item )
{
  m_toggle = &item;
} // get_toggle_status::set_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the toggle from which we take the status.
 */
const bear::engine::base_item*
bear::engine::get_toggle_status::get_toggle() const
{
  return m_toggle.get_item();
} // get_toggle_status::get_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
bear::expr::base_boolean_expression*
bear::engine::get_toggle_status::clone() const
{
  return new get_toggle_status(*this);
} // get_toggle_status::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
bool bear::engine::get_toggle_status::evaluate() const
{
  if ( m_toggle == (with_toggle*)NULL )
    {
      claw::logger << claw::log_warning
                   << "get_toggle_status: the toggle is NULL, the evaluation"
        " is 'false'." << std::endl;

      return false;
    }
  else
    return m_toggle->is_on();
} // get_toggle_status::evaluate()
