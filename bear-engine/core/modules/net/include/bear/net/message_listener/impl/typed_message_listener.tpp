/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::net::typed_message_listener class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The function to which are passed the messages.
 */
template<typename MessageType>
bear::net::typed_message_listener<MessageType>::typed_message_listener
( const callback_type f )
  : m_callback(f)
{

} // typed_message_listener::typed_message_listener()
      
/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this listener.
 */
template<typename MessageType>
typename bear::net::typed_message_listener<MessageType>::self_type*
bear::net::typed_message_listener<MessageType>::clone() const
{
  return new self_type(*this);
} // typed_message_listener::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Process a message.
 * \param m The message to process.
 */
template<typename MessageType>
void bear::net::typed_message_listener<MessageType>::do_process_message
( const message& m ) const
{
  const MessageType* real_message = dynamic_cast<const message_type*>(&m);

  CLAW_PRECOND( real_message != NULL );
  
  m_callback(*real_message);
} // typed_message_listener::do_process_message()
