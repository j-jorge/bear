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
#include "bear/engine/variable/var_map.hpp"

#include <claw/multi_type_map_visitor.hpp>

template class claw::multi_type_map<std::string, bear::engine::var_types>;
template
class claw::multi_type_map<std::string, bear::engine::var_map::signal_types>;

/*----------------------------------------------------------------------------*/
/**
 * \brief Call delete on a pointer to boost::signal.
 */
template<typename T>
void bear::engine::var_map::delete_signal::operator()
  ( const std::string& name, boost::signals2::signal<void (T)>* value ) const
{
  delete value;
} // var_map::delete_signal::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param m The map in which the signals are searched.
 */
bear::engine::var_map::trigger_signal::trigger_signal( const signal_map& m )
  : m_signals(m)
{
  // nothing to do
} // var_map::trigger_signal::trigger_signal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Trigger the signal corresponding to a given variable.
 * \param name The name of the variable.
 * \param value The value of the variable.
 */
template<typename T>
void bear::engine::var_map::trigger_signal::operator()
  ( const std::string& name, const T& value ) const
{
  typedef boost::signals2::signal<void (T)>* signal_type;

  if ( m_signals.exists<signal_type>(name) )
    (*m_signals.get<signal_type>(name))(value);
} // trigger_signal::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param m The map used as the reference for the variables to keep.
 * \param s The map in which the signals are searched.
 */
bear::engine::var_map::delete_signal_not_in::delete_signal_not_in
( const var_map& m, signal_map& s )
  : m_map(m), m_signals(s)
{
  // nothing to do
} // var_map::delete_signal_not_in::delete_signal_not_in()

/*----------------------------------------------------------------------------*/
/**
 * \brief Delete the signal associated with a variable, if needed.
 * \param name The name of the variable.
 * \param value The value of the variable.
 *
 * If there is no variable named \a name in m_map, the signal associated with
 * \a name in m_signals, if any, is deleted and removed.
 */
template<typename T>
void bear::engine::var_map::delete_signal_not_in::operator()
( const std::string& name, const T& value ) const
{
  if ( !m_map.exists<T>(name) )
    {
      typedef boost::signals2::signal<void (T)>* signal_type;

      if ( m_signals.exists<signal_type>(name) )
        {
          delete m_signals.get<signal_type>(name);
          m_signals.erase<signal_type>(name);
        }
    }
} // var_map::delete_signal_not_in::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::var_map::var_map()
{
  // nothing to do
} // var_map::var_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::var_map::~var_map()
{
  claw::multi_type_map_visitor v;
  delete_signal f;

  v.run( m_signals, f );
} // var_map::~var_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bear::engine::var_map::var_map( const var_map& that )
  : super(that)
{

} // var_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 * \param that The instance to copy from.
 */
bear::engine::var_map& bear::engine::var_map::operator=( const var_map& that )
{
  /// \todo Trigger a signal for the deleted variables too.
  claw::multi_type_map_visitor v;
  delete_signal_not_in remove(that, m_signals);
  v.run(*(super*)this, remove);

  super::operator=(that);

  trigger_signal update(m_signals);
  v.run((super&)that, update);

  return *this;
} // var_map::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy all the values of an other map.
 * \param m The other map.
 */
void bear::engine::var_map::set( const var_map& m )
{
  super::set(m);
} // set()
