/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A runtime setting is a debugging feature that allows to set the values
 *        of some settings at runtime.
 * \author Julien Jorge
 */
#ifndef __BEAR_RUNTIME_SETTING_HPP__
#define __BEAR_RUNTIME_SETTING_HPP__

#include "generic_items/class_export.hpp"

#include <claw/exception.hpp>
#include <claw/multi_type_map.hpp>
#include <string>

namespace bear
{
  /** \brief The types that can be modified at runtime. */
  typedef claw::meta::type_list_maker
  <int*, unsigned int*, bool*, double*, std::string*>::result setting_types;

  /** \brief The structure in which we store the settings. */
  typedef claw::multi_type_map<std::string, setting_types> setting_map;

  extern GENERIC_ITEMS_EXPORT setting_map g_global_settings;

#ifndef NDEBUG
  template<typename F>
  bool add_static_debug_settings( F f )
  {
    f();
    return true;
  } // add_static_settings()

  template<typename T>
  void add_debug_setting( std::string name, T var )
  {
    if ( g_global_settings.template exists<T>(name) )
      throw claw::exception( "Duplicate debug setting: '" + name + "'" );

    g_global_settings.template set<T>(name, var);
  }
#endif

} // namespace bear

#ifndef NDEBUG

#define BEAR_DEBUG_ADD_SETTING(type, var, id)                   \
  bear::add_debug_setting( #id, const_cast<type*>(&var) );

#define BEAR_DEBUG_REMOVE_SETTING(type, id)     \
  bear::g_global_settings.erase<type*>( #id );

#define BEAR_DEBUG_ADD_STATIC_SETTINGS(f)       \
  static bool static_settings = bear::add_static_debug_settings(f);

#else

#define BEAR_DEBUG_ADD_SETTING(type, var, id)
#define BEAR_DEBUG_REMOVE_SETTING(type, id)
#define BEAR_DEBUG_ADD_STATIC_SETTINGS(f)

#endif

#endif // __BEAR_RUNTIME_SETTING_HPP__
