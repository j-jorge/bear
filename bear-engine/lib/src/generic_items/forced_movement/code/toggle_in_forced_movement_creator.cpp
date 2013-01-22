/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::toggle_in_forced_movement_creator class.
 * \author Julien Jorge
 */
#include "generic_items/forced_movement/toggle_in_forced_movement_creator.hpp"

#include "engine/item_brick/with_toggle.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( toggle_in_forced_movement_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "bool".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::toggle_in_forced_movement_creator::set_bool_field
( const std::string& name, bool value )
{
  bool result = true;

  if ( name == "toggle_in_forced_movement_creator.use_toggle_duration" )
    m_movement.use_toggle_delay(value);
  else
    result = super::set_bool_field(name, value);

  return result;
} // toggle_in_forced_movement_creator::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "item".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::toggle_in_forced_movement_creator::set_item_field
( const std::string& name, engine::base_item* value )
{
  bool result = true;

  if ( (name == "toggle_in_forced_movement_creator.toggle")
       && (dynamic_cast<engine::with_toggle*>(value) != NULL) )
    m_movement.set_reference_point_on_center(*value);
  else
    result = super::set_item_field(name, value);

  return result;
} // toggle_in_forced_movement_creator::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all required fields are initialized.
 */
bool bear::toggle_in_forced_movement_creator::is_valid() const
{
  const engine::with_toggle* const toggle =
    dynamic_cast<engine::with_toggle*>(&m_movement.get_reference_item());

  return m_movement.has_reference_item() && (toggle != NULL)
    && super::is_valid();
} // toggle_in_forced_movement_creator::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::toggle_in_forced_movement_creator::build()
{
  set_forced_movement(m_movement);

  kill();
} // toggle_in_forced_movement_creator::build()
