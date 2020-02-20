/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A function object to get the value of a level variable.
 * \author Julien Jorge
 */
#ifndef __BEAR_ENGINE_LEVEL_VARIABLE_GETTER_HPP__
#define __BEAR_ENGINE_LEVEL_VARIABLE_GETTER_HPP__

#include <string>

namespace bear
{
  namespace engine
  {
    class level;

    /**
     * \brief A function object to get the value of a level variable.
     * \author Julien Jorge
     */
    template<typename T>
    class level_variable_getter
    {
    public:
      level_variable_getter();
      level_variable_getter(const level_variable_getter<T>& that);
      level_variable_getter
      ( const level* lvl, const std::string& var_name,
        const T& default_value = T() );

      void set_level( const level* lvl );

      void set_name( const std::string& n );
      const std::string& get_name() const;

      void set_default_value( const T& v );

      T operator()() const;

    private:
      /** \brief The level from which we take the variable. */
      const level* m_level;

      /** \brief The name of the variable. */
      std::string m_name;

      /** \brief The default value. */
      T m_default_value;

    }; // class level_variable_getter
  } // namespace engine
} // namespace bear

#include "bear/engine/function/impl/level_variable_getter.tpp"

#endif // __BEAR_ENGINE_LEVEL_VARIABLE_GETTER_HPP__
