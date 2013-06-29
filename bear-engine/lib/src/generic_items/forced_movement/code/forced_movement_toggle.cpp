/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::forced_movement_toggle class.
 * \author Julien Jorge
 */
#include "generic_items/forced_movement/forced_movement_toggle.hpp"

#include "universe/forced_movement/forced_movement.hpp"

#include "engine/item_brick/loader/forced_movement_applicator_loader.hpp"

BASE_ITEM_EXPORT( forced_movement_toggle, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::forced_movement_toggle::forced_movement_toggle()
  : m_apply_to_activator(false)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // forced_movement_toggle::forced_movement_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is well initialized.
 */
bool bear::forced_movement_toggle::is_valid() const
{
  return m_movement.is_valid();
} // forced_movement_toggle::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "bool".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::forced_movement_toggle::set_bool_field
( const std::string& name, bool value )
{
  bool result = true;

  if ( name == "forced_movement_toggle.apply_to_activator" )
    m_apply_to_activator = value;
  else
    result = super::set_bool_field(name, value);

  return result;
} // forced_movement_toggle::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::forced_movement_toggle::populate_loader_map
( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( engine::forced_movement_applicator_loader(m_movement) );
} // forced_movement_toggle::populate_loader_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the movement to the actors.
 * \param activator The activator of the toggle.
 */
void bear::forced_movement_toggle::on_toggle_on( engine::base_item* activator )
{
  m_movement.give_movement();

  if ( (activator != NULL) && m_apply_to_activator )
    activator->set_forced_movement( m_movement.get_movement() );
} // forced_movement_toggle::on_toggle_on()
