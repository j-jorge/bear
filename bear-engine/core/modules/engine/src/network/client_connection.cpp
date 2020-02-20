/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::client_connection class.
 * \author Julien Jorge
 */
#include "bear/engine/network/client_connection.hpp"

#include "bear/engine/network/message_factory.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param h The host to which the client is connected.
 * \param p The port through which the client is connected.
 */
bear::engine::client_connection::client_connection
( const std::string& host, unsigned int port )
  : m_host(host), m_port(port),
    m_client(host, port, message_factory::get_instance())
{

} // client_connection::client_connection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the host to which the client is connected.
 */
const std::string& bear::engine::client_connection::get_host() const
{
  return m_host;
} // client_connection::get_host()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the port through which the client is connected.
 */
unsigned int bear::engine::client_connection::get_port() const
{
  return m_port;
} // client_connection::get_port()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the client connected to the server.
 */
bear::net::client& bear::engine::client_connection::get_client()
{
  return m_client;
} // client_connection::get_client()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear the list of messages received by this client.
 */
void bear::engine::client_connection::clear_message_queue()
{
  m_message.clear();
} // client_connection::clear_message_queue()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the messages to be processed by this client.
 * \param m The messages.
 */
void bear::engine::client_connection::set_messages( const message_list& m )
{
  m_message = m;
} // client_connection::push_message()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the messages received by this client.
 */
std::vector<bear::net::message*>
bear::engine::client_connection::get_messages() const
{
  std::vector<net::message*> result;
  result.reserve(m_message.size());

  for ( message_list::const_iterator it=m_message.begin(); it!=m_message.end();
        ++it )
    {
      net::message_handle m = *it;
      net::message* p = &*m;

      result.push_back(p);
    }

  return result;
} // client_connection::get_messages()
