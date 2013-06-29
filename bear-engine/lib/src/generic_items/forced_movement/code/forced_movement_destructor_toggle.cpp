/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::forced_movement_destructor_toggle class.
 * \author Julien Jorge
 */
#include "generic_items/forced_movement/forced_movement_destructor_toggle.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( forced_movement_destructor_toggle, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::forced_movement_destructor_toggle::forced_movement_destructor_toggle()
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // forced_movement_destructor_toggle::forced_movement_destructor_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "list of item".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::forced_movement_destructor_toggle::set_item_list_field
( const std::string& name, const std::vector<engine::base_item*>& value )
{
  bool result = true;

  if ( name == "forced_movement_destructor_toggle.actor" )
    {
      m_actor.resize(value.size());
      for (std::size_t i=0; i!=value.size(); ++i)
        m_actor[i] = value[i];
    }
  else
    result = super::set_item_list_field(name, value);

  return result;
} // forced_movement_destructor_toggle::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all required fields are initialized.
 */
bool bear::forced_movement_destructor_toggle::is_valid() const
{
  bool result = super::is_valid();

  for (std::size_t i=0; result && (i!=m_actor.size()); ++i)
    result = m_actor[i] != NULL;

  return result;
} // forced_movement_destructor_toggle::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the movement to the actors.
 * \param activator The activator of the toggle.
 */
void bear::forced_movement_destructor_toggle::on_toggle_on
( engine::base_item* activator )
{
  for (std::size_t i=0; i!=m_actor.size(); ++i)
    if ( m_actor[i] != NULL )
      m_actor[i]->clear_forced_movement();
} // forced_movement_destructor_toggle::on_toggle_on()
