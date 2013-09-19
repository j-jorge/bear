/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
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

#include <sstream>
#include <typeinfo>

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

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a formatted and human readable representation of this expression.
 */
std::string bear::engine::get_toggle_status::formatted_string() const
{
  std::ostringstream result;

  result << "get_toggle_status( ";

  if ( m_toggle == (with_toggle*)NULL )
    result << "null";
  else
    result << typeid(*m_toggle).name();

  result << " [= " << evaluate() << "] )";

  return result.str();
} // get_toggle_status::formatted_string()
