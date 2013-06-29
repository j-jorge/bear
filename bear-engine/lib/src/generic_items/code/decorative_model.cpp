/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::decorative_model class.
 * \author Julien Jorge
 */
#include "generic_items/decorative_model.hpp"

BASE_ITEM_EXPORT( decorative_model, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is well prepared.
 */
bool bear::decorative_model::is_valid() const
{
  return ! m_initial_action.empty() && ! m_model_path.empty() 
    && super::is_valid();
} // decorative_model::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::decorative_model::build()
{
  super::build();

  set_model_actor( get_level_globals().get_model(m_model_path) );
  start_model_action( m_initial_action );
} // decorative_model::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type string.
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::decorative_model::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "decorative_model.model_file" )
    m_model_path = value;
  else if ( name == "decorative_model.initial_action" )
    m_initial_action = value;
  else
    result = super::set_string_field(name, value);

  return result;
} // bear::decorative_model::set_string_field()
