/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This implementation of resource pool allows to access resource files
 *        from a directory on disk.
 * \author Julien Jorge
 */
#ifndef __ENGINE_DIRECTORY_RESOURCE_POOL_HPP__
#define __ENGINE_DIRECTORY_RESOURCE_POOL_HPP__

#include <string>
#include <iostream>

#include "bear/engine/resource_pool/base_resource_pool.hpp"
#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief This implementation of resource pool allows to access resource
     *        files from a directory on disk.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT directory_resource_pool:
      public base_resource_pool
    {
    public:
      directory_resource_pool( const std::string& path );

      void get_file( const std::string& name, std::ostream& os );
      bool exists( const std::string& name ) const;

    private:
      bool find_file( const std::string& name, std::ifstream& f ) const;
      bool find_file_name_straight( std::string& name ) const;

    private:
      /** \brief The path of the directory where the resources are searched. */
      std::string m_path;

    }; // class directory_resource_pool
  } // namespace engine
} // namespace bear

#endif // __ENGINE_DIRECTORY_RESOURCE_POOL_HPP__
