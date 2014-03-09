/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the template methods of bear::engine::var_map.
 * \author Julien Jorge
 */

#include <claw/multi_type_map_visitor.hpp>
#include <claw/meta/type_list.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a signal to connect to be informed when the value of a given
 *        variable change.
 * \param name The name of the variable to listen.
 */
template<typename T>
boost::signals2::signal<void (T)>&
bear::engine::var_map::variable_changed( const std::string& name )
{
  typedef boost::signals2::signal<void (T)> signal_type;

  if ( !m_signals.exists<signal_type*>(name) )
    m_signals.set<signal_type*>( name, new signal_type() );

  return *m_signals.get<signal_type*>(name);
} // var_map::variable_changed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of a given variable.
 * \param k The name of the variable.
 * \param v The new value of the variable.
 */
template<typename T>
void bear::engine::var_map::set( const std::string& k, const T& v )
{
  bool signal = true;

  if ( exists<T>(k) )
    {
      T old_value = get<T>(k);
      super::set<T>(k, v);

      signal = ( old_value != v );
    }
  else
    super::set<T>(k, v);

  typedef boost::signals2::signal<void (T)> signal_type;

  if ( signal && m_signals.exists<signal_type*>(k) )
    (*m_signals.get<signal_type*>(k))(v);
} // var_map::set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply a function on each entry.
 * \param f The template function to call. The first argument must be compatible
 *        with std::string and the second one must be a parameter of the
 *        template.
 */
template<typename Function>
void bear::engine::var_map::for_each( Function f )
{
  claw::multi_type_map_visitor visitor;
  visitor.run( *(super*)this, f );
} // var_map::for_each()
