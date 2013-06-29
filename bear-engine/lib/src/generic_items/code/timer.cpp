/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::timer class.
 * \author Julien Jorge
 */
#include "generic_items/timer.hpp"

BASE_ITEM_EXPORT( timer, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::timer::timer()
: m_elapsed_time(0), m_initial_time(0), m_countdown(false), m_loop(false),
  m_toggle(NULL), m_loops_count(0), m_reset_when_reactivated(true)
{
  set_phantom(true);
  set_can_move_items(false);
  set_global(true);
} // timer::timer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param initial_time The initial time.
 * \param countdown Indicates if the timer is a countdown.
 */
bear::timer::timer(bear::universe::time_type initial_time, bool countdown)
: m_elapsed_time(0), m_initial_time(initial_time), 
  m_countdown(countdown), m_loop(false),
  m_toggle(NULL), m_loops_count(0), m_reset_when_reactivated(true)
{
  set_phantom(true);
  set_can_move_items(false);
  set_global(true);
} // timer::timer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item when turned on.
 * \param elapsed_time The elapsed time since the last call.
 */
void bear::timer::progress_on( bear::universe::time_type elapsed_time )
{
  super::progress_on(elapsed_time);

  m_elapsed_time += elapsed_time;

  if ( m_countdown && (m_elapsed_time >= m_initial_time) )
    {
      if ( m_toggle != (engine::with_toggle*)NULL )
        m_toggle->toggle(this);

      if ( m_loop )
        {
          m_elapsed_time -= m_initial_time;
          ++m_loops_count;
        }
      else
        toggle_off(this);
    }
} // timer::progress_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "bool".
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::timer::set_bool_field( const std::string& name, bool value )
{
  bool result = true;

  if ( name == "timer.countdown" )
    m_countdown = value;
  else if ( name == "timer.loop" )
    m_loop = value;
  else if ( name == "timer.reset_when_reactivated" )
    m_reset_when_reactivated = value;
  else
    result = super::set_bool_field( name, value );

  return result;
} // timer::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::timer::set_real_field( const std::string& name, double value )
{
  bool result(true);

  if (name == "timer.time")
    m_initial_time = value;
  else
    result = super::set_real_field( name, value );

  return result;
} // timer::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::timer::set_item_field( const std::string& name, engine::base_item* value )
{
  bool result(true);

  if (name == "timer.toggle")
    set_toggle( value );
  else
    result = super::set_item_field( name, value );

  return result;
} // timer::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the time.
 */
bear::universe::time_type bear::timer::get_time() const
{
  if ( m_countdown )
    return m_initial_time - m_elapsed_time;
  else
    return m_elapsed_time;
} // timer::get_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the initial time.
 */
bear::universe::time_type bear::timer::get_initial_time() const
{
  return m_initial_time;
} // timer::get_initial_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the elapsed time since the timer has been turned on.
 */
bear::universe::time_type bear::timer::get_elapsed_time() const
{
  return m_elapsed_time;
} // timer::get_elapsed_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return if the timer is a countdown.
 */
bool bear::timer::is_countdown() const
{
  return m_countdown;
} // timer::is_countdown()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell how many loops the countdown have done.
 */
std::size_t bear::timer::get_loops() const
{
  return m_loops_count;
} // timer::get_loops()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the toggle activated at the end of the countdown.
 */
bear::engine::base_item* bear::timer::get_toggle() const
{
  return dynamic_cast<engine::base_item*>(m_toggle.get_item());
} // timer::get_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the toggle to activate at the end of the countdown.
 * \param t The toggle.
 */
void bear::timer::set_toggle( engine::base_item* t )
{
  m_toggle = t;
} // timer::set_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn the timer on.
 * \param activator The item who activated this toggle.
 */
void bear::timer::on_toggle_on( engine::base_item* activator )
{
  super::on_toggle_on( activator );

  if ( m_reset_when_reactivated )
    {
      m_elapsed_time = 0;
      m_loops_count = 0;
    }
} // timer::on_toggle_on()
