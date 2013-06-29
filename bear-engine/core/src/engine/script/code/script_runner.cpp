/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::script_runner class.
 * \author Julien Jorge
 */
#include "engine/script/script_runner.hpp"

#include "engine/script/script_parser.hpp"

#include <claw/logger.hpp>
#include <claw/real_number.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Set an actor of the script.
 * \param name The name of the actor.
 * \param item The actor.
 * \pre \a item inherit from bear::engine::with_text_interface
 */
void bear::engine::script_runner::set_actor_item
( const std::string& name, base_item* item )
{
  m_context.set_actor_item(name, item);
} // script_runner::set_actor_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set an actor of the script.
 * \param name The name of the actor.
 * \param item The actor.
 * \pre \a item inherit from text_interface::base_exportable
 */
void bear::engine::script_runner::set_actor
( const std::string& name, text_interface::base_exportable* item )
{
  m_context.set_actor(name, item);
} // script_runner::set_actor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a script.
 * \param s The path to the script to load.
 */
bool bear::engine::script_runner::load_script( const std::string& s )
{
  script_parser parser;
  bool result = parser.run( m_sequence, s );
  reset();

  m_context.set_actor("script", this);

  return result;
} // script_runner::load_script()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go back to the beginning of the script.
 */
void bear::engine::script_runner::reset()
{
  m_date = 0;
  m_current_call = m_sequence.begin();
} // script_runner::reset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we are done with this script.
 */
bool bear::engine::script_runner::finished() const
{
  return m_current_call == m_sequence.end();
} // script_runner::finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play some actions of the script.
 * \param duration How long do we play the script.
 */
void bear::engine::script_runner::play( universe::time_type duration )
{
  if ( m_date == 0 )
    on_script_started();

  m_date += duration;
  bool done(false);

  while ( !done && (m_current_call != m_sequence.end()) )
    if ( claw::real_number<universe::time_type>(m_current_call->date) > m_date )
      done = true;
    else
      {
        play_action();

        ++m_current_call;
      }
} // script_runner::play()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the total duration of the script.
 */
bear::universe::time_type
bear::engine::script_runner::get_script_duration() const
{
  if ( m_sequence.empty() )
    return 0;
  else
    {
      call_sequence::const_iterator it = m_sequence.end();
      --it;
      return it->date;
    }
} // script_runner::get_script_duration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the context.
 */
const bear::engine::script_context&
bear::engine::script_runner::get_context() const
{
  return m_context;
} // script_runner::get_context()

/*----------------------------------------------------------------------------*/
/**
 * \brief Extra processing by subclasses at the beginning of the script.
 */
void bear::engine::script_runner::on_script_started()
{
} // script_runner::on_script_started()

/*----------------------------------------------------------------------------*/
/**
 * \brief Extra processing by subclasses at the end of the script.
 */
void bear::engine::script_runner::on_script_stopped()
{
} // script_runner::on_script_stopped()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform that the script is finished.
 */
void bear::engine::script_runner::end()
{
  on_script_stopped();
} // script_runner::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play current action of the script.
 */
void bear::engine::script_runner::play_action()
{
  text_interface::base_exportable* actor =
    m_context.get_actor( m_current_call->call.get_actor_name() );

  if ( actor == NULL )
    claw::logger << claw::log_error << "Unknown actor '"
                 << m_current_call->call.get_actor_name() << "' at date "
                 << m_current_call->date << std::endl;
  else
    actor->execute
      ( m_current_call->call.get_method_name(),
        m_current_call->call.get_arguments(), m_context );
} // script_runner::play_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Export the methods of the class.
 */
void bear::engine::script_runner::init_exported_methods()
{
  TEXT_INTERFACE_CONNECT_METHOD_0( bear::engine::script_runner, end, void );
} // script_runner::init_exported_methods()

TEXT_INTERFACE_IMPLEMENT_METHOD_LIST(bear::engine::script_runner)
