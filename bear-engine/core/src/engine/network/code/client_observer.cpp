/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::client_observer class.
 * \author Sebastien Angibaud
 */
#include "engine/network/client_observer.hpp"

#include "engine/network/client_connection.hpp"
#include "net/message/message.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 * \param c The client of which we listen the messages.
 */
bear::engine::client_observer::client_observer( client_connection* c )
  : m_client(c)
{

} // client_observer::client_observer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the messages and dispatch them to the listeners.
 */
void bear::engine::client_observer::process_message()
{
  if ( m_client == NULL )
    return;

  std::vector<net::message*> messages = m_client->get_messages();
  
  for ( unsigned int i = 0; i != messages.size(); ++i )
    {
      const listener_map::const_iterator listener =
        m_listeners.find(messages[i]->get_name());

      if ( listener != m_listeners.end() )
	listener->second.process_message(*(messages[i]));
    }
} // client_observer::process_message()
