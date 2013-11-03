/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::system_api class for Unix
 *        platforms.
 * \author Julien Jorge
 */
#include "engine/system/system_api.hpp"

#include <boost/algorithm/string/replace.hpp>

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Opens an object (file, url, …) with the default application.
 * \param object The object to open.
 */
void bear::engine::system_api::open( const std::string& object )
{
  const std::string argument
    ( boost::algorithm::replace_all_copy( object, "'", "'\"'\"'" ) );

  const std::string command("xdg-open '" + argument + "'");
  
  const int exec_result( system(command.c_str()) );

  if ( exec_result == -1 )
    claw::logger << claw::log_error << "Failed to open '" << object
                 << "' with command: " << command
                 << std::endl;
} // system_api::open()
