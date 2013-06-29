/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::delayed_kill_item class.
 * \author Sebastien Angibaud
 */
#include "generic_items/delayed_kill_item.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( delayed_kill_item, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::delayed_kill_item::delayed_kill_item()
  : m_duration(0), m_start_duration(0)
{
  set_phantom(true);
  set_can_move_items(false);
  set_global(true);
  set_artificial(true);
} // delayed_kill_item::delayed_kill_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 */
void bear::delayed_kill_item::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  m_start_duration += elapsed_time;

  if ( m_start_duration >= m_duration )
    {
      std::vector<handle_type>::iterator it;
      for (it = m_items.begin(); it != m_items.end(); ++it)
        if ( *it != (base_item*)NULL )
          (*it)->kill();

      kill();
    }
} // delayed_kill_item::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type list of <*base_item>.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::delayed_kill_item::set_item_list_field
( const std::string& name, const std::vector<base_item*>& value )
{
  bool result = false;

  if ( name == "delayed_kill_item.killing_items" )
    {
      m_items.resize(value.size());

      for ( std::size_t i=0; i!=value.size(); ++i )
        m_items[i] = value[i];

      result = true;
    }
  else
    result = super::set_item_list_field( name, value );

  return result;
} // delayed_kill_item::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param value The value of the field.
 */
bool bear::delayed_kill_item::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "delayed_kill_item.duration" )
    m_duration = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // delayed_kill_item::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set duration until kill item.
 * \param value The value of duration.
 */
void bear::delayed_kill_item::set_duration( double value )
{
  m_duration = value;
} // delayed_kill_item::set_duration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item to kill.
 * \param item The item to kill.
 */
void bear::delayed_kill_item::add_item( base_item* item )
{
  m_items.push_back(item);
} // delayed_kill_item::add_item()
