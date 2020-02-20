/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::browser_launcher_toggle class.
 * \author Sebastien Angibaud
 */
#include "bear/generic_items/browser_launcher_toggle.hpp"

#include "bear/engine/system/system_api.hpp"

BASE_ITEM_EXPORT( browser_launcher_toggle, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::browser_launcher_toggle::browser_launcher_toggle()
{
  
} // browser_launcher_toggle::browser_launcher_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type string.
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::browser_launcher_toggle::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "browser_launcher_toggle.url" )
    m_url = value;
  else
    result = super::set_string_field( name, value );

  return result;
} // browser_launcher_toggle::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play the sample.
 * \param activator (ignored) The item that activates the toggle, if any.
 */
void bear::browser_launcher_toggle::on_toggle_on
( engine::base_item* activator )
{
  engine::system_api::open( m_url );
} // browser_launcher_toggle::on_toggle_on()
