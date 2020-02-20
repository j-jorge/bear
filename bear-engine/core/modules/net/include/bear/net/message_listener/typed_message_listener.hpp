/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A message listener for a given message type.
 * \author Julien Jorge
 */
#ifndef __NET_TYPED_MESSAGE_LISTENER_HPP__
#define __NET_TYPED_MESSAGE_LISTENER_HPP__

#include "bear/net/class_export.hpp"
#include "bear/net/message_listener/base_message_listener.hpp"

namespace bear
{
  namespace net
  {
    class message;

    /**
     * \brief A message listener for a given message type.
     * \author Julien Jorge
     */
    template<typename MessageType>
    class typed_message_listener :
      public base_message_listener
    {
    public:
      /** \brief The type of the messages processed by this listener. */
      typedef MessageType message_type;

      /** \brief The type of the callback called with the messages. */
      typedef boost::function<void (const message_type&)> callback_type;

      /** \brief The type of this class. */
      typedef typed_message_listener<message_type> self_type;

    public:
      typed_message_listener( const callback_type f );
      
      virtual self_type* clone() const;

    private:
      virtual void do_process_message( const message& m ) const;

    private:
      /** \brief The function to which are passed the messages. */
      callback_type m_callback;

    }; // class typed_message_listener

  } // namespace net
} // namespace bear

#include "bear/net/message_listener/impl/typed_message_listener.tpp"

#endif // __NET_TYPED_MESSAGE_LISTENER_HPP__
