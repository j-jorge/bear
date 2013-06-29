/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::killer class.
 * \author Julien Jorge
 */
#include "generic_items/killer.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( killer, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::killer::killer()
  : m_kill_activator(false)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // killer::killer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type list of \c base_item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::killer::set_item_list_field
( const std::string& name, const std::vector<base_item*>& value )
{
  bool result = false;

  if ( name == "killer.items" )
    {
      m_items.resize(value.size());

      for ( std::size_t i=0; i!=value.size(); ++i )
        m_items[i] = value[i];
      result = true;
    }
  else
    result = super::set_item_list_field( name, value );

  return result;
} // killer::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param value The value of the field.
 */
bool bear::killer::set_bool_field( const std::string& name, bool value )
{
  bool result = true;

  if ( name == "killer.kill_activator" )
    m_kill_activator = value;
  else
    result = super::set_bool_field(name, value);

  return result;
} // killer::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Activate the toggle.
 * \param activator The item that activates the toggle.
 */
void bear::killer::toggle_on( base_item* activator )
{
  for ( ; !m_items.empty(); m_items.pop_back() )
    if (m_items.back() != (base_item*)NULL)
      m_items.back()->kill();

  if ( m_kill_activator && (activator != NULL) )
    activator->kill();
} // killer::toggle_on()
