/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::input::finger_status class.
 * \author Julien Jorge
 */
#include "bear/input/finger_status.hpp"

#include "bear/input/input_listener.hpp"
#include "bear/input/system.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the status of the finger.
 */
void bear::input::finger_status::read()
{
  const finger& f = system::get_instance().get_finger();

  m_events = f.get_events();
} // finger_status::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send the status of the finger to an input_listener.
 * \param listener The listener to pass informations to.
 */
void bear::input::finger_status::scan_inputs( input_listener& listener ) const
{
  for ( event_list::const_iterator it = m_events.begin(); it != m_events.end();
        ++it )
    listener.finger_action( *it );
} // finger_status::scan_inputs()
