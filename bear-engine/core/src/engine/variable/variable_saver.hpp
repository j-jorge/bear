/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A function object that saves a variable in a stream.
 * \author Julien Jorge
 */
#ifndef __ENGINE_VARIABLE_SAVER_HPP__
#define __ENGINE_VARIABLE_SAVER_HPP__

#include <boost/regex.hpp>
#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief A function object that saves a variable in a stream.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT variable_saver
    {
    public:
      variable_saver( std::ostream& os, boost::regex pattern );

      template<typename T>
      void operator()( const std::string& name, const T& value ) const;

    private:
      template<typename T>
      const T& escape( const T& value ) const;

      std::string escape( const std::string& value ) const;

    private:
      /** \brief The stream in which the variable is saved. */
      std::ostream& m_output;

      /** \brief A regular expression that must be verified by the variable's
          name to be serialised. */
      const boost::regex m_pattern;

    }; // class variable_saver

  } // namespace engine
} // namespace bear

#include "engine/variable/impl/variable_saver.tpp"

#endif // __ENGINE_VARIABLE_SAVER_HPP__
