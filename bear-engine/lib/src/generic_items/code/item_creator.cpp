/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::item_creator class.
 * \author Sébastien Angibaud
 */
#include "generic_items/item_creator.hpp"

#include "universe/forced_movement/forced_sequence.hpp"
#include "universe/forced_movement/forced_goto.hpp"
#include "universe/forced_movement/forced_join.hpp"

BASE_ITEM_EXPORT( item_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::item_creator::~item_creator()
{
  for ( unsigned int index = 0; index != m_items.size(); ++index )
    delete m_items[index];
} // item_creator::item_creator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item.
 */
void bear::item_creator::build()
{
  super::build();

  for ( unsigned int index = 0; index != m_items.size(); ++index )
    {
      base_item* item = m_items[index];
      m_items[index] = item->clone();
      item->kill();
    }
} // item_creator::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::item_creator::set_item_list_field
( const std::string& name, const std::vector<base_item*>& value )
{
  bool result(true);

  if ( name == "item_creator.items" )
    {
      m_items.clear();
      for ( unsigned int index = 0; index != value.size(); ++index )
        m_items.push_back(value[index]);
    }
  else
    result = super::set_item_list_field(name, value);

  return result;
} // item_creator::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all fields have a correct value.
 */
bool bear::item_creator::is_valid() const
{
  for ( std::size_t i=0; i!=m_items.size(); ++i )
    if ( m_items[i]->get_insert_as_static() )
      return false;

  return !m_items.empty() && super::is_valid();
} // item_creator::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief This method is called when the toggles switches on.
 * \param activator The item that activates the toggle, if any.
 */
void bear::item_creator::on_toggle_on( base_item* activator )
{
  super::on_toggle_on( activator );

  for ( unsigned int index = 0; index != m_items.size(); ++index )
    new_item( *(m_items[index]->clone()) );
} // item_creator::on_toggle_on()
