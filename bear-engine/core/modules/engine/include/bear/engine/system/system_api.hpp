/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The system_api class provides functions to do operations in the system
 *        running the game.
 * \author Julien Jorge
 */
#ifndef __ENGINE_SYSTEM_API_HPP__
#define __ENGINE_SYSTEM_API_HPP__

#include "bear/engine/class_export.hpp"

#include <string>

namespace bear
{
  namespace engine
  {
    /**
     * \brief The system_api class provides functions to do operations in the
     *        system running the game.
     */
    class ENGINE_EXPORT system_api
    {
    public:
      static void open( const std::string& object );

    }; // class system_api

  } // namespace engine
} // namespace bear

#endif // __ENGINE_SYSTEM_API_HPP__
