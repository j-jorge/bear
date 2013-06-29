/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::net::server class.
 * \author Julien Jorge
 */
#include "net/server.hpp"

#include "net/message/message.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param port The port on which the server listens.
 * \param client_read_time_limit How long do we wait when asking a client for
 *        its content.
 */
bear::net::server::server( unsigned int port, int client_read_time_limit )
  : m_server(port), m_client_read_time_limit(client_read_time_limit)
{

} // server::server()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::net::server::~server()
{
  for ( client_list::const_iterator it=m_clients.begin(); it!=m_clients.end();
        ++it )
    delete *it;
} // server::~server()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of connections to which this server sends the messages.
 */
std::size_t bear::net::server::get_connection_count() const
{
  return m_clients.size();
} // server::get_connection_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Dispatch a message to the clients.
 * \param m The message to dispatch.
 */
void bear::net::server::dispatch_message( const message& m )
{
  for ( std::size_t i=0; i!=get_connection_count(); ++i )
    send_message( i, m );
} // server::dispatch_message()
      
/*----------------------------------------------------------------------------*/
/**
 * \brief Send a message to one client.
 * \param client_id The identifier of the client to which the message is sent.
 * \param m The message to send.
 */
void bear::net::server::send_message( std::size_t client_id, const message& m )
{
  CLAW_PRECOND( client_id < m_clients.size() );

  client_list::const_iterator it=m_clients.begin();
  std::advance(it, client_id);

  client_type& client = **it;
  client << m.get_name() << '\n' << m << std::endl;
} // server::send_message()
      
/*----------------------------------------------------------------------------*/
/**
 * \brief Update the list of alive clients.
 */
void bear::net::server::check_for_new_clients()
{
  bool check_client = true;
  while (check_client)
    {
      client_pointer c = new client_type(m_client_read_time_limit);
      m_server.accept( *c, 0 );

      if ( c->is_open() )
        {
          m_clients.push_back(c);
          on_new_client(m_clients.size() - 1);
        }
      else
        {
          delete c;
          check_client = false;
        }
    }
} // server::check_for_new_clients()
