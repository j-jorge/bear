/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::add_script_actor class.
 * \author Sebastien Angibaud
 */
#include "generic_items/script/add_script_actor.hpp"

BASE_ITEM_EXPORT( add_script_actor, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::add_script_actor::add_script_actor()
  : m_actor(NULL)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // add_script_actor::add_script_actor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Give the actor at the script director.
 * \param director The script director.
 */
void bear::add_script_actor::set_actor(script_director& director)
{
  if ( m_actor != (base_item*)NULL )
    director.set_actor_item(m_actor_name, m_actor.get());
} // add_script_actor::set_actor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c base_item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::add_script_actor::set_item_field
( const std::string& name, base_item* value )
{
  bool ok = true;

  if (name == "add_script_actor.actor")
    m_actor = value;
  else
    ok = super::set_item_field(name, value);

  return ok;
} // add_script_actor::set_item_field()
/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c <string>.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::add_script_actor::set_string_field
( const std::string& name, const std::string& value )
{
  bool ok = true;

  if (name == "add_script_actor.actor_name")
    m_actor_name = value;
  else
    ok = super::set_string_field(name, value);

  return ok;
} // add_script_actor::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is correctly initialized.
 */
bool bear::add_script_actor::is_valid() const
{
  return ( m_actor != (base_item*)NULL ) &&
    !m_actor_name.empty() && super::is_valid();
} // add_script_actor::is_valid()
