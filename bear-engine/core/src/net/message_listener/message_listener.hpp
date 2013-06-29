/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class that process messages.
 * \author Julien Jorge
 */
#ifndef __NET_MESSAGE_LISTENER_HPP__
#define __NET_MESSAGE_LISTENER_HPP__

#include "net/class_export.hpp"

namespace bear
{
  namespace net
  {
    class base_message_listener;
    class message;

    /**
     * \brief A class that process messages.
     * \author Julien Jorge
     */
    class NET_EXPORT message_listener
    {
    public:
      message_listener();
      message_listener( const message_listener& that );
      message_listener( const base_message_listener& listener );
      ~message_listener();

      void process_message( const message& m ) const;
      message_listener& operator=( const message_listener& that );

      void swap( message_listener& that );

    private:
      /** \brief The implementation of this instance. */
      base_message_listener* m_impl;

    }; // class message_listener

  } // namespace net
} // namespace bear

#endif // __NET_MESSAGE_LISTENER_HPP__
