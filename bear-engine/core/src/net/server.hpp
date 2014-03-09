/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A server is an object that can dispatch messages to clients.
 * \author Julien Jorge
 */
#ifndef __NET_SERVER_HPP__
#define __NET_SERVER_HPP__

#include "net/class_export.hpp"

#include <list>
#include <claw/non_copyable.hpp>
#include <claw/socket_server.hpp>
#include <boost/signals2.hpp>

namespace bear
{
  namespace net
  {
    class message;

    /**
     * \brief A server is an object that can dispatch messages to clients.
     * \author Julien Jorge
     */
    class NET_EXPORT server:
      private claw::pattern::non_copyable
    {
    private:
      /** \brief The type of a clients connected to this server. */
      typedef claw::net::socket_stream client_type;

      /** \brief The type of a pointer on a client connected to this server. */
      typedef client_type* client_pointer;

      /** \brief The list of clients connected to this server. */
      typedef std::list<client_pointer> client_list;

    public:
      server( unsigned int port, int client_read_time_limit = 0 );
      ~server();

      std::size_t get_connection_count() const;

      void dispatch_message( const message& m );
      void send_message( std::size_t client_id, const message& m );

      boost::signals2::signal<void (std::size_t)> on_new_client;

      void check_for_new_clients();

    private:
      /** \brief This is the socket in which the messages are dispatched and
          from which the new clients are taken. */
      claw::net::socket_server m_server;

      /** \brief Those are the clients to which the messages are dispatched. */
      client_list m_clients;

      /** \brief How long do we wait when asking a client for its content. */
      int m_client_read_time_limit;

    }; // class server

  } // namespace net
} // namespace bear

#endif // __NET_SERVER_HPP__
