/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::game_network class.
 * \author Julien Jorge
 */
#include "engine/game_network.hpp"

#include "engine/network/client_connection.hpp"
#include "engine/network/message/sync.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::game_network::game_network()
  : m_sync_id(0), m_min_horizon(1), m_active(false)
{

} // game_network::game_network()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::game_network::~game_network()
{
  for ( server_map::const_iterator it=m_server.begin(); it!=m_server.end();
        ++it )
    delete it->second;
} // game_network::~game_network()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of clients managed by this instance.
 */
std::size_t bear::engine::game_network::get_client_count() const
{
  return m_future.size();
} // game_network::get_client_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the representation of the future of a given client.
 * \param client_index The index of the client for which the future is
 *        returned.
 */
const bear::engine::client_future&
bear::engine::game_network::get_future( std::size_t client_index ) const
{
  CLAW_PRECOND( client_index < m_future.size() );

  client_future_map::const_iterator it=m_future.begin();
  std::advance( it, client_index );
  return it->second;
} // game_network::get_future()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the connection of a client at a given index.
 * \param client_index The index of the client whose connection is returned.
 */
const bear::engine::client_connection&
bear::engine::game_network::get_connection( std::size_t client_index ) const
{
  CLAW_PRECOND( client_index < m_future.size() );

  client_future_map::const_iterator it=m_future.begin();
  std::advance( it, client_index );
  return *it->first;
} // game_network::get_connection()


/*----------------------------------------------------------------------------*/
/**
 * \brief Get the minimum number of message lists that can effectively be given
 *        to the clients.
 */
std::size_t bear::engine::game_network::get_horizon() const
{
  if ( m_future.empty() )
    return 0;

  std::size_t result( std::numeric_limits<std::size_t>::max() );

  for ( client_future_map::const_iterator it=m_future.begin();
        it != m_future.end();
        ++it )
    result = std::min( result, it->second.get_horizon() );

  return result;
} // game_network::get_horizon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the minimum number of message lists that must be ready in order to
 *        pass the oldest one to the clients.
 */
std::size_t bear::engine::game_network::get_min_horizon() const
{
  return m_min_horizon;
} // game_network::get_min_horizon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the minimum number of message lists that must be ready in order to
 *        pass the oldest one to the clients.
 * \param m The minimum number of message lists.
 */
void bear::engine::game_network::set_min_horizon( std::size_t m )
{
  m_min_horizon = m;
} // game_network::set_min_horizon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send a message through a service.
 * \param service_name The name of the service.
 * \param m The message to send.
 */
void bear::engine::game_network::send_message
( const std::string& service_name, net::message& m ) const
{
  CLAW_PRECOND( m_server.find(service_name) != m_server.end() );

  m.set_date( m_sync_id );

  m_server.find(service_name)->second->dispatch_message(m);
} // game_network::send_message()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a new service with a given name and listening on a given port.
 * \param name The name of the service.
 * \param port The port on which the service listens.
 */
void bear::engine::game_network::create_service
( const std::string& name, unsigned int port )
{
  if ( m_server.find(name) == m_server.end() )
    {
      net::server* s = new net::server(port);
      s->on_new_client.connect
        ( boost::bind( &game_network::on_new_client, this, s,
                       boost::placeholders::_1 ) );
      m_server[name] = s;
    }
} // game_network::create_service()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an observer for a client.
 * \param host The host to which the client is connected.
 * \param port The port through which the client listens.
 */
bear::engine::client_observer
bear::engine::game_network::connect_to_service
( const std::string& host, unsigned int port )
{
  client_connection* result = NULL;

  for ( client_list::const_iterator it=m_client.begin();
        (it != m_client.end()) && (result == NULL);
        ++it )
    {
      client_connection* connection = *it;

      if ( (connection->get_host() == host)
           && (connection->get_port() == port) )
        result = connection;
    }

  if ( result == NULL )
    result = create_new_client(host, port);

  return client_observer(result);
} // game_network::connect_to_service()

/*----------------------------------------------------------------------------*/
/**
 * \brief Synchronize the servers and the clients.
 */
bool bear::engine::game_network::synchronize()
{
  for ( server_map::const_iterator it=m_server.begin(); it!=m_server.end();
        ++it )
    it->second->check_for_new_clients();

  for ( client_list::iterator it=m_client.begin(); it!=m_client.end(); ++it )
    pull_client_messages(*it);

  m_active = set_client_messages();

  return m_active;
} // game_network::synchronize()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send a sync message on all servers.
 */
void bear::engine::game_network::send_synchronization()
{
  if ( !m_active )
    return;

  sync s(m_sync_id + m_min_horizon, true);
  ++m_sync_id;

  for ( server_map::const_iterator it=m_server.begin(); it!=m_server.end();
        ++it )
    it->second->dispatch_message( s );
} // game_network::send_synchronize()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a new client.
 * \param host The host to which the client is connected.
 * \param port The port through which the client listens.
 * \return A pointer to the client added in the m_client list.
 */
bear::engine::client_connection*
bear::engine::game_network::create_new_client
( const std::string& host, unsigned int port )
{
  client_connection* result = new client_connection(host, port);
  m_client.push_back( result );
  m_future[result] = client_future( get_min_horizon() );

  return result;
} // game_network::create_new_client()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the messages of a client until a sync message is received.
 * \param c The client from which the messages are read.
 */
void bear::engine::game_network::pull_client_messages( client_connection* c )
{
  net::message_handle m = c->get_client().pull_message();

  while ( m != NULL )
    {
      m_future[c].push_message( m );
      m = c->get_client().pull_message();
    }
} // game_network::pull_client_messages()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the messages of a client connection for the current date.
 */
bool bear::engine::game_network::prepare_clients()
{
  std::size_t total(0);
  std::size_t ready(0);

  for ( client_list::iterator it=m_client.begin(); it!=m_client.end(); ++it )
    {
      ++total;

      client_connection* const c = *it;
      c->clear_message_queue();

      if ( m_future[c].get_horizon() == 0 )
        m_filling.insert(c);
      else
        {
          if ( m_future[c].get_horizon() >= m_min_horizon )
            m_filling.erase(c);

          const sync& s = m_future[c].get_sync_message(0);
      
          if ( s.is_active_sync() && (s.get_id() == m_sync_id) )
            ++ready;
        }
    }

  if ( ready == total )
    return m_filling.empty();
  else
    return false;
} // game_network::prepare_clients()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the messages of the client connections for the current date.
 */
bool bear::engine::game_network::set_client_messages()
{
  const bool ready = prepare_clients();

  if ( ready )
    for ( client_list::iterator it=m_client.begin(); it!=m_client.end(); ++it )
      {
        client_connection* const c = *it;
        c->set_messages( m_future[c].next() );
      }

  return ready;
} // game_network::set_client_messages()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send some messages to initialize a new client.
 * \param s The server that has accepted the client.
 * \param client_id The identifier of the client as provided by the server.
 */
void bear::engine::game_network::on_new_client
( net::server* s, std::size_t client_id )
{
  for ( std::size_t i=0; i!=m_min_horizon; ++i )
    s->send_message( client_id, sync( m_sync_id + i, true ) );
} // game_network::on_new_client()
