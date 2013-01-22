/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The structure in which we store the level variables or game variables.
 * \author Julien Jorge
 */
#ifndef __ENGINE_VAR_MAP_HPP__
#define __ENGINE_VAR_MAP_HPP__

#include "engine/class_export.hpp"

#include <claw/multi_type_map.hpp>
#include <claw/multi_type_map_visitor.hpp>
#include <claw/meta/type_list.hpp>

#include <string>
#include <boost/signal.hpp>

namespace bear
{
  namespace engine
  {
    /** \brief The types supported by the engine. */
    typedef claw::meta::type_list_maker
    <int, unsigned int, bool, double, std::string>::result var_types;

    /**
     * \brief The structure in which we store the level variables or game
     *        variables.
     */
    class ENGINE_EXPORT var_map:
      public claw::multi_type_map<std::string, var_types>
    {
    private:
      /** \brief The type of the parent map. */
      typedef claw::multi_type_map<std::string, var_types> super;

      /** \brief The types of the signals used to observe the variables. */
      typedef claw::meta::type_list_maker
      < boost::signal<void (int)>*, boost::signal<void (unsigned int)>*,
        boost::signal<void (bool)>*, boost::signal<void (double)>*,
        boost::signal<void (std::string)>* >::result signal_types;

      /**
       * \brief The map containing the signals associated with the variables.
       */
      typedef claw::multi_type_map<std::string, signal_types> signal_map;

      /**
       * \brief A function object that deletes the signal associated with a
       *        variable.
       * \author Julien Jorge
       */
      class delete_signal
      {
      public:
        template<typename T>
        void operator()
        ( const std::string& name, boost::signal<void (T)>* value ) const;

      }; // class delete_signal

      /**
       * \brief A function object that triggers the signal associated with a
       *        variable.
       * \author Julien Jorge
       */
      class trigger_signal
      {
      public:
        trigger_signal( const signal_map& m );

        template<typename T>
        void operator()
        ( const std::string& name, const T& value ) const;

      private:
        /** \brief The map in which the signals are searched. */
        const signal_map& m_signals;

      }; // class trigger_signal

      /**
       * \brief A function object that deletes the the signals for the variables
       *        not declared in a given var_map.
       * \author Julien Jorge
       */
      class delete_signal_not_in
      {
      public:
        delete_signal_not_in( const var_map& m, signal_map& s );

        template<typename T>
        void operator()
        ( const std::string& name, const T& value ) const;

      private:
        /** \brief The var_map in which the variables are searched. */
        const var_map& m_map;

        /** \brief The map in which the signals are searched. */
        signal_map& m_signals;

      }; // class delete_signal_not_in()

    public:
      var_map();
      ~var_map();
      var_map( const var_map& that );

      var_map& operator=( const var_map& that );

      template<typename T>
      boost::signal<void (T)>& variable_changed( const std::string& name );

      template<typename T>
      void set( const std::string& k, const T& v );

      void set( const var_map& m );

      template<typename Function>
      void for_each( Function f );

    private:
      /** \brief The signals triggered when the value of a variable change. */
      signal_map m_signals;

    }; // class var_map

  } // namespace engine
} // namespace bear

#include "engine/variable/impl/var_map.tpp"

#endif // __ENGINE_VAR_MAP_HPP__
