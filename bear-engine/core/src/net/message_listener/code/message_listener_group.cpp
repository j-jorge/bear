/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::net::message_listener_group class.
 * \author Julien Jorge
 */
#include "net/message_listener/message_listener_group.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Dispatch a message to the listeners.
 * \param m The message to dispatch.
 */
void
bear::net::message_listener_group::process_message( const message& m ) const
{
  for ( listener_list_type::const_iterator it=m_listener.begin();
        it!=m_listener.end(); ++it )
    it->process_message(m);
} // message_listener_group::process_message()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a message listener.
 * \param listener The listener to add.
 */
void bear::net::message_listener_group::add( const message_listener& listener )
{
  m_listener.push_back(listener);
} // message_listener::add()
