/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A function object that copies a variable in a var map.
 * \author Sebastien Angibaud
 */
#ifndef __ENGINE_VARIABLE_COPY_HPP__
#define __ENGINE_VARIABLE_COPY_HPP__

#include <boost/regex.hpp>
#include "engine/variable/var_map.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief A function object that copies a variable in a var map.
     * \author Sebastien Angibaud
     */
    class ENGINE_EXPORT variable_copy
    {
    public:
      explicit variable_copy( var_map& vars, const boost::regex& pattern );

      template<typename T>
      void operator()( const std::string& name, const T& value ) const;

    private:
      template<typename T>
      const T& escape( const T& value ) const;

      std::string escape( const std::string& value ) const;

    private:
      /** \brief The var map in which we copy varaiable. */
      var_map& m_vars;

      /** \brief A regular expression that must be verified by the variable's
          name to be serialised. */
      const boost::regex& m_pattern;

    }; // class variable_copy

  } // namespace engine
} // namespace bear

#include "engine/variable/impl/variable_copy.tpp"

#endif // __ENGINE_VARIABLE_COPY_HPP__
