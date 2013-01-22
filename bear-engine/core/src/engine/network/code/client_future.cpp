/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::client_future class.
 * \author Julien Jorge
 */
#include "engine/network/client_future.hpp"

#include "engine/network/message/sync.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::client_future::client_future()
  : m_prefered_horizon( std::numeric_limits<std::size_t>::max() )
{

} // client_future::client_future()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param h The size of the horizon up to which the message lists closed with
 *          a not active sync message are still stored.
 */
bear::engine::client_future::client_future( std::size_t h )
  : m_prefered_horizon(h)
{

} // client_future::client_future()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a message in the future of the client.
 * \param m The message to add.
 */
void bear::engine::client_future::push_message( net::message_handle m )
{
  m_pending.push_back(m);

  sync* s = dynamic_cast<sync*>(&*m);

  if ( s != NULL )
    {
      if ( s->is_active_sync() || (m_future.size() < m_prefered_horizon) )
        m_future.push_back( m_pending );

      m_pending.clear();
    }
} // client_future::push_message()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of message lists ready to be processed.
 */
std::size_t bear::engine::client_future::get_horizon() const
{
  return m_future.size();
} // client_future::get_horizon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size of the horizon up to which the message lists closed with
 *        a not active sync message are still stored.
 * \param h The prefered horizon.
 */
void bear::engine::client_future::set_prefered_horizon( std::size_t h )
{
  m_prefered_horizon = h;
} // client_future::set_prefered_horizon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sync message that closes a given message list.
 * \param i The index of the message list for which we want the message.
 */
const bear::engine::sync&
bear::engine::client_future::get_sync_message( std::size_t i ) const
{
  future::const_iterator it = m_future.begin();
  std::advance(it, i);
  net::message_handle m = it->back();

  return *static_cast<bear::engine::sync*>(&*m);
} // client_future::get_sync_message()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the next message list to process and remove it from the future.
 */
bear::engine::client_future::message_list bear::engine::client_future::next()
{
  CLAW_PRECOND( get_horizon() != 0 );

  message_list result;

  result.swap( m_future.front() );
  m_future.pop_front();

  return result;
} // client_future::next()
