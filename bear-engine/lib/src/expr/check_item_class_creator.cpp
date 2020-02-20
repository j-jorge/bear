/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::check_item_class_creator class.
 * \author Julien Jorge
 */
#include "bear/generic_items/expr/check_item_class_creator.hpp"

#include "bear/expr/boolean_constant.hpp"
#include "bear/engine/expr/check_item_class.hpp"
#include "bear/engine/item_brick/with_trigger.hpp"

#include "bear/engine/export.hpp"

#include <claw/logger.hpp>

BASE_ITEM_EXPORT( check_item_class_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "string".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::check_item_class_creator::set_string_list_field
( const std::string& name, const std::vector<std::string>& value )
{
  bool result = true;

  if ( name == "check_item_class_creator.class_name" )
    m_class_name = value;
  else
    result = super::set_string_list_field(name, value);

  return result;
} // check_item_class_creator::set_string_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "item".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::check_item_class_creator::set_item_field
( const std::string& name, engine::base_item* value )
{
  bool result = true;

  if ( name == "check_item_class_creator.collision_data")
    {
      engine::with_trigger* t( dynamic_cast<engine::with_trigger*>(value) );

      if ( t != NULL )
        m_collision_data = t;
      else
        claw::logger << claw::log_error << name
                     << ": the item is not of type 'engine::with_trigger'."
                     << std::endl;
    }
  else
    result = super::set_item_field(name, value);

  return result;
} // check_item_class_creator::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all required fields are initialized.
 */
bool bear::check_item_class_creator::is_valid() const
{
  return !m_class_name.empty() && (m_collision_data != NULL)
    && super::is_valid();
} // check_item_class_creator::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::check_item_class_creator::build()
{
  kill();
} // check_item_class_creator::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the expression created by this item.
 */
bear::expr::boolean_expression
bear::check_item_class_creator::do_get_expression() const
{
  expr::boolean_expression result( expr::boolean_constant(false) );

  for ( std::size_t i=0; i!=m_class_name.size(); ++i )
    {
      engine::check_item_class e;
      e.set_class_name( m_class_name[i] );
      e.set_collision_data( m_collision_data->get_collision_in_expression() );
      result = result || e;
    }

  return result;
} // check_item_class_creator::do_get_expression()
