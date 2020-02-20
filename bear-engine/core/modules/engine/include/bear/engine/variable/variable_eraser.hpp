/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A function object that erases the variables whose name match a given
 *        pattern.
 * \author Julien Jorge
 */
#ifndef __ENGINE_VARIABLE_ERASER_HPP__
#define __ENGINE_VARIABLE_ERASER_HPP__

#include "bear/engine/variable/var_map.hpp"

#include <boost/regex.hpp>

namespace bear
{
  namespace engine
  {
    /**
     * \brief A function object that erases the variables whose name match a
     *        given pattern.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT variable_eraser
    {
    public:
      explicit variable_eraser( var_map& m, boost::regex pattern );

      template<typename T>
      void operator()( const std::string& name, const T& value ) const;

    private:
      /** \brief The map from which the variables are erased. */
      var_map& m_map;

      /** \brief A regular expression that must be verified by the variable's
          name to be serialised. */
      const boost::regex m_pattern;

    }; // class variable_eraser

  } // namespace engine
} // namespace bear

#include "bear/engine/variable/impl/variable_eraser.tpp"

#endif // __ENGINE_VARIABLE_ERASER_HPP__
