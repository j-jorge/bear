/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class represent a level variable or a game variable.
 * \author Julien Jorge
 */
#ifndef __ENGINE_VARIABLE_HPP__
#define __ENGINE_VARIABLE_HPP__

#include "engine/variable/base_variable.hpp"
#include "engine/variable/var_map.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief A variable of a known type.
     * \author Julien Jorge
     */
    template<typename T>
    class variable:
      public base_variable
    {
    public:
      explicit variable( const std::string& name );
      variable( const std::string& name, const T& val );

      const T& get_value() const;

      virtual void assign_value_to( var_map& m ) const;
      virtual void get_value_from( const var_map& m );
      virtual bool exists( const var_map& m ) const;

    private:
      /** \brief The value of the variable. */
      T m_value;

    }; // class variable

  } // namespace engine
} // namespace bear

#include "engine/variable/impl/variable.tpp"

#endif // __ENGINE_VARIABLE_HPP__
