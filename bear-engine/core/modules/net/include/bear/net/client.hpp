/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A client is an object that can connect to a server to receive its
 *        messages.
 * \author Julien Jorge
 */
#ifndef __NET_CLIENT_HPP__
#define __NET_CLIENT_HPP__

#include "bear/net/class_export.hpp"

#include "bear/net/connection_status.hpp"
#include "bear/net/message/message.hpp"
#include "bear/net/message_factory.hpp"

#include <string>
#include <claw/smart_ptr.hpp>
#include <claw/socket_stream.hpp>
#include <claw/non_copyable.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>


namespace bear
{
  namespace net
  {
    typedef claw::memory::smart_ptr<message> message_handle;

    class connection_thread;

    /**
     * \brief A client is an object that can connect to a server to receive its
     *        messages.
     * \author Julien Jorge
     */
    class NET_EXPORT client:
      private claw::pattern::non_copyable
    {
      friend class connection_thread;

    public:
      client
        ( const std::string& host, unsigned int port,
          const message_factory& f, int read_time_limit = 0 );
      ~client();

      connection_status get_status() const;

      message_handle pull_message();

    private:
      void connect();
      void set_stream( claw::net::socket_stream* stream );

    private:
      /** \brief The host to which we connect. */
      const std::string m_host;

      /** \brief The port through which we connect. */
      const unsigned int m_port;

      /** \brief How long do we wait when nothing comes while reading the
          stream. */
      const int m_read_time_limit;

      /** \brief This is the socket from which the messages are read. */
      claw::net::socket_stream* m_stream;

      /** \brief The factory to use to instantiate the received messages. */
      const message_factory& m_message_factory;

      /** \brief The thread currently trying to create the connection. */
      boost::thread* m_connection;

      /** \brief This mutex is used to avoid accessing the m_stream and changing
          it at the same time. */
      mutable boost::mutex m_stream_mutex;

    }; // class client

  } // namespace net
} // namespace bear

#endif // __NET_CLIENT_HPP__
