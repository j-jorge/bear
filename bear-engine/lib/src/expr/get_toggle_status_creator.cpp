/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::get_toggle_status_creator class.
 * \author Julien Jorge
 */
#include "bear/generic_items/expr/get_toggle_status_creator.hpp"

#include "bear/engine/item_brick/with_toggle.hpp"

#include "bear/engine/export.hpp"

#include <claw/logger/logger.hpp>

BASE_ITEM_EXPORT( get_toggle_status_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "item".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::get_toggle_status_creator::set_item_field
( const std::string& name, engine::base_item* value )
{
  bool result = true;

  if ( name == "get_toggle_status_creator.toggle")
    {
      engine::with_toggle* t( dynamic_cast<engine::with_toggle*>(value) );

      if ( t != NULL )
        m_expr.set_toggle( *value );
      else
        claw::logger << claw::log_error << name
                     << ": the item is not of type 'engine::with_toggle'."
                     << std::endl;
    }
  else
    result = super::set_item_field(name, value);

  return result;
} // get_toggle_status_creator::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::get_toggle_status_creator::build()
{
  kill();
} // get_toggle_status_creator::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all required fields are initialized.
 */
bool bear::get_toggle_status_creator::is_valid() const
{
  return (m_expr.get_toggle() != NULL) && super::is_valid();
} // get_toggle_status_creator::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the expression created by this item.
 */
bear::expr::boolean_expression
bear::get_toggle_status_creator::do_get_expression() const
{
  return m_expr;
} // get_toggle_status_creator::do_get_expression()
