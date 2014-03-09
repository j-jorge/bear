#ifdef WIN32

/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::system_api class for Windows
 *        platforms.
 * \author Julien Jorge
 */
#include "engine/system/system_api.hpp"

#include <windef.h>
#include <objbase.h>
#include <winuser.h>
#include <shellapi.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Opens an object (file, url, …) with the default application.
 * \param object The object to open.
 */
void bear::engine::system_api::open( const std::string& object )
{
  CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
  ShellExecute(NULL, "open", object.c_str(), NULL, NULL, SW_SHOWDEFAULT);
} // system_api::open()
#endif
