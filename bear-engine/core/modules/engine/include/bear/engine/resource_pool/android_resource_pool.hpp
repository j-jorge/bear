/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This implementation of resource pool allows to access resource files
 *        from a android on disk.
 * \author Julien Jorge
 */
#ifdef __ANDROID__

#ifndef __ENGINE_ANDROID_RESOURCE_POOL_HPP__
#define __ENGINE_ANDROID_RESOURCE_POOL_HPP__

#include "bear/engine/resource_pool/base_resource_pool.hpp"

#include <string>
#include <iostream>

#include <android/asset_manager.h>

namespace bear
{
  namespace engine
  {
    /**
     * \brief This implementation of resource pool allows to access resource
     *        files from a android on disk.
     * \author Julien Jorge
     */
    class android_resource_pool:
      public base_resource_pool
    {
    public:
      void get_file( const std::string& name, std::ostream& os );
      bool exists( const std::string& name ) const;
    
    private:
      AAssetManager* get_asset_manager() const;

    }; // class android_resource_pool
  } // namespace engine
} // namespace bear

#endif // __ENGINE_ANDROID_RESOURCE_POOL_HPP__

#endif // __ANDROID__
