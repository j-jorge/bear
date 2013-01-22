/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::navigator_launcher_toggle class.
 * \author Sebastien Angibaud
 */
#include "generic_items/navigator_launcher_toggle.hpp"

#ifdef _WIN32
#include <windef.h>
#include <objbase.h>
#include <winuser.h>
#include <shellapi.h>
#endif

BASE_ITEM_EXPORT( navigator_launcher_toggle, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::navigator_launcher_toggle::navigator_launcher_toggle()
{
  
} // navigator_launcher_toggle::navigator_launcher_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type string.
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::navigator_launcher_toggle::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "navigator_launcher_toggle.url" )
    m_url = value;
  else
    result = super::set_string_field( name, value );

  return result;
} // navigator_launcher_toggle::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play the sample.
 * \param activator (ignored) The item that activates the toggle, if any.
 */
void bear::navigator_launcher_toggle::on_toggle_on
( engine::base_item* activator )
{
#ifdef _WIN32
  CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
  ShellExecute(NULL, "open", m_url.c_str(), NULL, NULL, SW_SHOWDEFAULT);
#else // _WIN32
  std::string command("xdg-open ");
  command += m_url;
  
  system(command.c_str());
#endif
} // navigator_launcher_toggle::on_toggle_on()
