/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The connection_status enum list the various states in which a
 *        connection can be.
 * \author Julien Jorge
 */
#ifndef __NET_CONNECTION_STATUS_HPP__
#define __NET_CONNECTION_STATUS_HPP__

namespace bear
{
  namespace net
  {
    /**
     * \brief The connection_status enum list the various states in which a
     *        connection can be.
     * \author Julien Jorge
     */
    enum connection_status
      {
        /** \brief The connection is on its way, you should wait. */
        connection_status_connecting,

        /** \brief The connection is ready, you can use it. */
        connection_status_connected,

        /** \brief There is no connection, you should create one. */
        connection_status_disconnected

      }; // enum connection_status

  } // namespace net
} // namespace bear

#endif // __NET_CONNECTION_STATUS_HPP__
