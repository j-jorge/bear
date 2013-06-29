/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The client_future class manages the lists of the future messages that
 *        will be applied to the clients.
 * \author Julien Jorge
 */
#ifndef __ENGINE_CLIENT_FUTURE_HPP__
#define __ENGINE_CLIENT_FUTURE_HPP__

#include "engine/network/client_observer.hpp"
#include "engine/class_export.hpp"

#include "net/client.hpp"
#include "net/server.hpp"

#include <map>
#include <set>

namespace bear
{
  namespace engine
  {
    class client_connection;
    class sync;

    /**
     * \brief The client_future class manages the lists of the future messages
     *        that will be applied to the clients.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT client_future
    {
    public:
      /** \brief The type of the collection in which the pending messages are
          kept. */
      typedef std::list<net::message_handle> message_list;

    private:
      /** \brief The type of the collection in which the future message lists
          are stored. */
      typedef std::list<message_list> future;

    public:
      client_future();
      explicit client_future( std::size_t h );
      void push_message( net::message_handle m );

      std::size_t get_horizon() const;
      const sync& get_sync_message( std::size_t i ) const;

      void set_prefered_horizon( std::size_t h );

      message_list next();

    private:
      /** \brief The message lists that will be returned by the next calls to
          next(). */
      future m_future;

      /** \brief The list in construction. It will be added at the end of
          m_future when a sync message will be received. */
      message_list m_pending;

      /** \brief The size of m_future up to which the message lists closed with
          a not active sync message are still stored. */
      std::size_t m_prefered_horizon;

    }; // class client_future
  } // namespace engine
} // namespace bear

#endif // __ENGINE_CLIENT_FUTURE_HPP__
