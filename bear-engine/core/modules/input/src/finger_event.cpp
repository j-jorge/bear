/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the input::finger_event class.
 * \author Julien Jorge
 */
#include "bear/input/finger_event.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a pressed event.
 * \param p The position where the finger has been pressed.
 * \param finger_id The identifier of the finger emitting this event.
 */
bear::input::finger_event
bear::input::finger_event::create_pressed_event
( const position_type& p, int finger_id )
{
  finger_event e( finger_event_pressed, finger_id );
  e.m_position = p;

  return e;
} // finger_event::create_pressed_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a released event.
 * \param p The position where the finger has been released.
 * \param finger_id The identifier of the finger emitting this event.
 */
bear::input::finger_event
bear::input::finger_event::create_released_event
( const position_type& p, int finger_id )
{
  finger_event e( finger_event_released, finger_id );
  e.m_position = p;

  return e;
} // finger_event::create_released_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a motion event.
 * \param p The position where the event occurred.
 * \param d The distance of the motion.
 * \param finger_id The identifier of the finger emitting this event.
 */
bear::input::finger_event
bear::input::finger_event::create_motion_event
( const position_type& p, int finger_id, const position_type& d )
{
  finger_event e( finger_event_motion, finger_id );
  e.m_position = p;
  e.m_distance = d;

  return e;
} // finger_event::create_motion_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the identifier of the finger emitting this event.
 */
int bear::input::finger_event::get_finger_id() const
{
  return m_finger_id;
} // finger_event::get_finger_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the type of the event.
 */
bear::input::finger_event::event_type
bear::input::finger_event::get_type() const
{
  return m_type;
} // finger_event::get_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the position where the event occurred.
 */
const bear::input::position_type&
bear::input::finger_event::get_position() const
{
  return m_position;
} // finger_event::get_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns a copy of this event as if it occurred at a given position.
 * \param p The position of the returned event.
 */
bear::input::finger_event
bear::input::finger_event::at_position( const position_type& p ) const
{
  finger_event result(*this);
  result.m_position = p;

  return result;
} // finger_event::at_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the distance of the motion.
 */
const bear::input::position_type&
bear::input::finger_event::get_distance() const
{
  return m_distance;
} // finger_event::get_distance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs an event of an unknown type.
 * \param t The type of the event.
 * \param finger_id The identifier of the finger emitting this event.
 */
bear::input::finger_event::finger_event( event_type t, int finger_id )
  : m_type(t), m_finger_id( finger_id ), m_position(0, 0), m_distance(0, 0)
{
  
} // finger_event::finger_event()
