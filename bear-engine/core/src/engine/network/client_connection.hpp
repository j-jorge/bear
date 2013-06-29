/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The client_connection class keep the informations of the client
 *        connections to the servers.
 * \author Julien Jorge
 */
#ifndef __ENGINE_CLIENT_CONNECTION_HPP__
#define __ENGINE_CLIENT_CONNECTION_HPP__

#include "net/client.hpp"

#include "engine/class_export.hpp"

#include <list>
#include <string>
#include <vector>

namespace bear
{
  namespace engine
  {
    /**
     * \brief The client_connection class keep the informations of the client
     *        connections to the servers.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT client_connection
    {
    private:
      /** \brief The type of the list containing the messages. */
      typedef std::list<net::message_handle> message_list;

    public:
      client_connection( const std::string& host, unsigned int port );

      const std::string& get_host() const;
      unsigned int get_port() const;
      net::client& get_client();

      void clear_message_queue();
      void set_messages( const message_list& m );

      std::vector<net::message*> get_messages() const;

    private:
      /** \brief The host to which the client is connected. */
      const std::string m_host;
        
      /** \brief The port through which the client is connected. */
      const unsigned int m_port;

      /** \brief The client that does the connection to the server. */
      net::client m_client;

      /** \brief The messages received by this client at the last
          synchronization. */
      message_list m_message;

    }; // class client_connection
  } // namespace engine
} // namespace bear

#endif // __ENGINE_CLIENT_CONNECTION_HPP__
