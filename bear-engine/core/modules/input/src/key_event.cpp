/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::input::key_event class.
 * \author Julien Jorge
 */
#include "bear/input/key_event.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param t The type of the event.
 * \param info Informations about the key.
 */
bear::input::key_event::key_event( event_type t, const key_info& info )
  : m_type(t), m_info(info)
{

} // key_event::key_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the type of the event.
 */
bear::input::key_event::event_type bear::input::key_event::get_type() const
{
  return m_type;
} // key_event::get_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the informations about the key.
 */
const bear::input::key_info& bear::input::key_event::get_info() const
{
  return m_info;
} // key_event::get_info()
