/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::net::client class.
 * \author Julien Jorge
 */
#include "net/client.hpp"

#include "net/connection_task.hpp"
#include "net/message/message.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param host The host to which this client is connected.
 * \param port The port to which this client is connected.
 * \param f The factory to use to instantiate the received messages. The
 *        instance must live longer than this.
 * \param read_time_limit How long do we wait when nothing comes.
 */
bear::net::client::client
( const std::string& host, unsigned int port, const message_factory& f,
  int read_time_limit )
  : m_host(host), m_port(port), m_read_time_limit(read_time_limit),
    m_stream(NULL), m_message_factory(f), m_connection(NULL)
{
  connect();
} // client::client()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::net::client::~client()
{
  boost::mutex::scoped_lock lock(m_stream_mutex);

  delete m_connection;
  delete m_stream;
} // client::client()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the status of the connection.
 */
bear::net::connection_status bear::net::client::get_status() const
{
  boost::mutex::scoped_lock lock(m_stream_mutex);

  if ( m_stream != NULL )
    {
      if ( m_stream->is_open() )
        return connection_status_connected;
      else
        return connection_status_disconnected;
    }
  else if ( m_connection == NULL )
    return connection_status_disconnected;
  else
    return connection_status_connecting;
} // client::get_status()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the oldest message message received by this client and still not
 *        retrieved.
 */
bear::net::message_handle bear::net::client::pull_message()
{
  if ( get_status() == connection_status_disconnected )
    connect();

  if ( get_status() != connection_status_connected )
    return NULL;

  std::string message_name;
  message* result = NULL;
  m_stream->clear();

  while ( (result == NULL) && std::getline(*m_stream, message_name) )
    if ( !message_name.empty() )
      {
        result = m_message_factory.create(message_name);
        *m_stream >> *result;
      }

  return result;
} // client::pull_message()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the connection.
 */
void bear::net::client::connect()
{
  const connection_task connection
    ( boost::bind( &client::set_stream, this, _1 ),
      m_host, m_port, m_read_time_limit );
  //m_connection = new boost::thread(connection);
  connection();
} // client::connect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the stream used by this client.
 * \param stream The stream.
 */
void bear::net::client::set_stream( claw::net::socket_stream* stream )
{
  boost::mutex::scoped_lock lock(m_stream_mutex);

  delete m_stream;
  m_stream = stream;

  delete m_connection;
  m_connection = NULL;
} // client::set_stream()

