/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::toggle_group class.
 * \author Julien Jorge
 */
#include "bear/generic_items/toggle_group.hpp"

BASE_ITEM_EXPORT( toggle_group, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type list of items.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::toggle_group::set_item_list_field
( const std::string& name, const std::vector<base_item*>& value )
{
  bool ok = true;

  if (name == "toggle_group.toggles")
    {
      for (std::size_t i=0; i!=value.size(); ++i)
        insert(value[i]);
    }
  else
    ok = super::set_item_list_field(name, value);

  return ok;
} // toggle_group::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert a toggle in the group.
 * \param t The toggle to insert.
 */
void bear::toggle_group::insert( engine::base_item* t )
{
  m_toggles.push_back(t);
} // toggle_group::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn on the toggles.
 * \param activator (ignored) The item that activates the toggle, if any.
 */
void bear::toggle_group::on_toggle_on( engine::base_item* activator )
{
  handle_list_type remaining;

  for ( ; !m_toggles.empty(); m_toggles.pop_front() )
    if ( m_toggles.front() != (engine::with_toggle*)NULL )
      {
        remaining.push_back(m_toggles.front());
        m_toggles.front()->toggle_on(activator);
      }

  std::swap(m_toggles, remaining);
} // toggle_group::on_toggle_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn off the toggles.
 * \param activator (ignored) The item that activates the toggle, if any.
 */
void bear::toggle_group::on_toggle_off( engine::base_item* activator )
{
  handle_list_type remaining;

  for ( ; !m_toggles.empty(); m_toggles.pop_front() )
    if ( m_toggles.front() != (engine::with_toggle*)NULL )
      {
        remaining.push_back(m_toggles.front());
        m_toggles.front()->toggle_off(activator);
      }

  std::swap(m_toggles, remaining);
} // toggle_group::on_toggle_off()
