/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::client_observer class.
 * \author Sebastien Angibaud
 */
#include "bear/engine/network/client_observer.hpp"

#include "bear/net/message_listener/typed_message_listener.hpp"
#include "bear/net/message_listener/message_listener.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief The client want to subscribe for a given message type.
 * \param f The fonction called for each received messages.
 */
template<typename MessageType>
void bear::engine::client_observer::subscribe
( boost::function<void (const MessageType&)> f )
{
  m_listeners[MessageType::static_get_name()].add
    ( net::message_listener(net::typed_message_listener<MessageType>(f)) );
} // client_observer::()
