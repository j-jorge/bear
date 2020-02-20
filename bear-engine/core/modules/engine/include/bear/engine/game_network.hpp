/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The class managing the access to the network used by the game.
 * \author Julien Jorge
 */
#ifndef __ENGINE_GAME_NETWORK_HPP__
#define __ENGINE_GAME_NETWORK_HPP__

#include "bear/engine/network/client_future.hpp"
#include "bear/engine/network/client_observer.hpp"
#include "bear/engine/class_export.hpp"

#include "bear/net/client.hpp"
#include "bear/net/server.hpp"

#include <map>
#include <set>

namespace bear
{
  namespace engine
  {
    class client_connection;

    /**
     * \brief The class managing the access to the network used by the game.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT game_network
    {
    private:
      /** \brief The type of the map containing the server associated with each
          service name. */
      typedef std::map<std::string, net::server*> server_map;

      /** \brief The list of the client connections to the servers. */
      typedef std::list<client_connection*> client_list;

      /** \brief The type of the map associating the pending messages to each
          client. */
      typedef std::map<client_connection*, client_future> client_future_map;

    public:
      game_network();
      ~game_network();

      std::size_t get_client_count() const;

      const client_future& get_future( std::size_t client_index ) const;
      const client_connection& get_connection( std::size_t client_index ) const;

      std::size_t get_horizon() const;
      std::size_t get_min_horizon() const;
      void set_min_horizon( std::size_t m );

      void send_message
        ( const std::string& service_name, net::message& m ) const;
      void create_service( const std::string& name, unsigned int port );

      client_observer
        connect_to_service( const std::string& host, unsigned int port );

      bool synchronize();
      void send_synchronization();

    private:
      client_connection*
        create_new_client( const std::string& host, unsigned int port );

      void pull_client_messages( client_connection* c );

      bool prepare_clients();
      bool set_client_messages();

      void on_new_client( net::server* s, std::size_t client_id );

    private:
      /** \brief The services provided by the local game. */
      server_map m_server;

      /** \brief The connections to the servers. */
      client_list m_client;

      /** \brief The pending messages of each client. */
      client_future_map m_future;

      /** \brief The last id given to the sync message. */
      std::size_t m_sync_id;

      /** \brief The minimum horizon to consider the synchronization
          successful. */
      std::size_t m_min_horizon;

      /** \brief The clients for which we are waiting to have enough
          messages. */
      std::set<client_connection*> m_filling;

      /** \brief Tell if at the last call to synchronize all clients received
          the messages for the active iteration. */
      bool m_active;

    }; // class game_network
  } // namespace engine
} // namespace bear

#endif // __ENGINE_GAME_NETWORK_HPP__
