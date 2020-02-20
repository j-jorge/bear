/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::net::connection_task class.
 * \author Julien Jorge
 */
#include "bear/net/connection_task.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The function to which the resulting stream is passed when ready.
 * \param host The host to which this connection_task is connected.
 * \param port The port to which this connection_task is connected.
 * \param read_time_limit How long do we wait when nothing comes.
 */
bear::net::connection_task::connection_task
( callback_type f, const std::string& host, unsigned int port,
  int read_time_limit )
  : m_callback(f), m_host(host), m_port(port),
    m_read_time_limit(read_time_limit)
{

} // connection_task::connection_task()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start the connection and pass the result to the callback.
 */
void bear::net::connection_task::operator()() const
{
  claw::net::socket_stream* result =
    new claw::net::socket_stream(m_host.c_str(), m_port);
  /*bool stop = false;

    while ( !stop && !result->is_open() );*/

  result->set_read_time_limit(m_read_time_limit);
  m_callback(result);
} // connection_task::operator()()
