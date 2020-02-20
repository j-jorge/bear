/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The connection_task starts a connection with a server and pass the
 *        resulting socket to a given client.
 * \author Julien Jorge
 */
#ifndef __NET_CONNECTION_TASK_HPP__
#define __NET_CONNECTION_TASK_HPP__

#include "bear/net/class_export.hpp"

#include <string>
#include <boost/function.hpp>
#include <claw/socket_stream.hpp>

namespace bear
{
  namespace net
  {
    class client;

    /**
     * \brief The connection_task starts a connection with a server and pass
     *        the resulting socket to a given client.
     * \author Julien Jorge
     */
    class NET_EXPORT connection_task
    {
    public:
      /** \brief The type of the function called when the stream is ready. */
      typedef boost::function<void (claw::net::socket_stream*)> callback_type;

    public:
      connection_task
        ( callback_type f, const std::string& host, unsigned int port,
          int read_time_limit );

      void operator()() const;

    private:
      /** \brief The function called when the stream is ready. */
      callback_type m_callback;
      
      /** \brief The host to which we connect. */
      const std::string m_host;

      /** \brief The port through which we connect. */
      const unsigned int m_port;

      /** \brief How long do we wait when nothing comes while reading the
          stream. */
      const int m_read_time_limit;

    }; // class connection_task

  } // namespace net
} // namespace bear

#endif // __NET_CONNECTION_TASK_HPP__
