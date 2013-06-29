/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::clone_toggle class.
 * \author Julien Jorge
 */
#include "generic_items/clone_toggle.hpp"

#include "engine/game.hpp"
#include "engine/resource_pool.hpp"
#include "engine/variable/variable.hpp"

BASE_ITEM_EXPORT( clone_toggle, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::clone_toggle::~clone_toggle()
{
  for ( std::size_t i=0; i!=m_items.size(); ++i )
    delete m_items[i];
} // clone_toggle::~clone_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type list of items.
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::clone_toggle::set_item_list_field
( const std::string& name, const std::vector<engine::base_item*>& value )
{
  bool result = true;

  if ( name == "clone_toggle.items" )
    {
      for ( std::size_t i=0; i!=value.size(); ++i )
        if ( value[i] != NULL )
          {
            m_items.push_back( value[i]->clone() );
            value[i]->kill();
          }
    }
  else
    result = super::set_item_list_field( name, value );

  return result;
} // clone_toggle::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play the sample.
 * \param activator (ignored) The item that activates the toggle, if any.
 */
void bear::clone_toggle::on_toggle_on( engine::base_item* activator )
{
  for ( std::size_t i=0; i!=m_items.size(); ++i )
    new_item( *m_items[i]->clone() );
} // clone_toggle::on_toggle_on()
