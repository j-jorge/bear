/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The base interface for the resource pools.
 * \author Julien Jorge
 */
#ifndef __ENGINE_BASE_RESOURCE_POOL_HPP__
#define __ENGINE_BASE_RESOURCE_POOL_HPP__

#include <string>
#include <iostream>

#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The base interface for the resource pools.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT base_resource_pool
    {
    public:
      virtual ~base_resource_pool() {}

      virtual void get_file( const std::string& name, std::ostream& os ) = 0;
      virtual bool exists( const std::string& name ) const = 0;

    }; // class base_resource_pool
  } // namespace engine
} // namespace bear

#endif // __ENGINE_BASE_RESOURCE_POOL_HPP__
