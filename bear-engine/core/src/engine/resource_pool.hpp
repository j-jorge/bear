/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The resource pool allows to access resource files regardless if they
 *        are in a resource archive or in a folder.
 * \author Julien Jorge
 */
#ifndef __ENGINE_RESOURCE_POOL_HPP__
#define __ENGINE_RESOURCE_POOL_HPP__

#include <vector>
#include <string>
#include <iostream>
#include <claw/basic_singleton.hpp>

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class base_resource_pool;

    /**
     * \brief The resource pool stores the resource files.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT resource_pool :
      public claw::pattern::basic_singleton<resource_pool>
    {
    private:
      /** \brief The type of the parent class. */
      typedef claw::pattern::basic_singleton<resource_pool> super;

    public:
      // Must be redefined to work correctly with dynamic libraries.
      // At least under Windows with MinGW.
      static resource_pool& get_instance();

      ~resource_pool();

      void add_path( const std::string& path );
      void add_pool( base_resource_pool* pool );

      void get_file( const std::string& name, std::ostream& os );
      bool exists( const std::string& name ) const;

    private:
      /** \brief Pools in which the files are searched. */
      std::vector<base_resource_pool*> m_pool;

    }; // class resource_pool
  } // namespace engine
} // namespace bear

#endif // __ENGINE_RESOURCE_POOL_HPP__
