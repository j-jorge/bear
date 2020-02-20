/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A function object to get the value of a game variable.
 * \author Julien Jorge
 */
#ifndef __BEAR_ENGINE_GAME_VARIABLE_GETTER_HPP__
#define __BEAR_ENGINE_GAME_VARIABLE_GETTER_HPP__

#include <string>

namespace bear
{
  namespace engine
  {
    /**
     * \brief A function object to get the value of a game variable.
     * \author Julien Jorge
     */
    template<typename T>
    class game_variable_getter
    {
    public:
      game_variable_getter();
      game_variable_getter(const game_variable_getter<T>& that);
      explicit game_variable_getter
      ( const std::string& var_name, const T& default_value = T() );

      void set_name( const std::string& n );
      const std::string& get_name() const;

      void set_default_value( const T& v );

      T operator()() const;

    private:
      /** \brief The name of the variable. */
      std::string m_name;

      /** \brief The default value. */
      T m_default_value;

    }; // class game_variable_getter
  } // namespace engine
} // namespace bear

#include "bear/engine/function/impl/game_variable_getter.tpp"

#endif // __BEAR_ENGINE_GAME_VARIABLE_GETTER_HPP__
