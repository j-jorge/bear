/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class containing a set of dynamic libraries from which we can get
 *        functions.
 * \author Julien Jorge
 */
#ifndef __ENGINE_LIBRARIES_POOL_HPP__
#define __ENGINE_LIBRARIES_POOL_HPP__

#include <string>
#include <list>
#include <claw/dynamic_library.hpp>

#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The libraries_pool is a class containing a set of dynamic
     *        libraries from which we can get functions.
     *
     * \author Julien Jorge
     */
    class libraries_pool
    {
    private:
      /** \brief The type of the list of libraries. */
      typedef std::list<claw::dynamic_library*> libraries_list;

    public:
      ENGINE_EXPORT ~libraries_pool();

      ENGINE_EXPORT
      void add_library( const std::string& name, bool current_program = false );
      ENGINE_EXPORT bool have_symbol( const std::string& symbol ) const;

      template<typename T>
      T get_symbol( const std::string& name ) const;

    private:
      /** \brief The libraries stored in this pool. */
      libraries_list m_libraries;

    }; // class libraries_pool
  } // namespace engine
} // namespace bear

#include "bear/engine/impl/libraries_pool.tpp"

#endif // __ENGINE_LIBRARIES_POOL_HPP__
