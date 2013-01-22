/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::communication::messageable class.
 * \author Julien Jorge
 */
#include "communication/messageable.hpp"
#include "communication/post_office.hpp"

#include <claw/assert.hpp>

/*---------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::communication::messageable::messageable()
  : m_name(post_office::no_name)
{

} // messageable::messageable()

/*---------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of this item.
 */
bear::communication::messageable::messageable( const std::string& name )
  : m_name(name)
{

} // messageable::messageable()

/*---------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 *
 * The new instance has no name.
 */
bear::communication::messageable::messageable( const messageable& that )
  : super(that), m_name(post_office::no_name)
{

} // messageable::messageable()

/*---------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::communication::messageable::~messageable()
{

} // messageable::~messageable()

/*---------------------------------------------------------------------------*/
/**
 * \brief Set/change the name of the item.
 */
void bear::communication::messageable::set_name(const std::string& name)
{
  m_name = name;
} // messageable::set_name()

/*---------------------------------------------------------------------------*/
/**
 * \brief Get the name of the item.
 */
const std::string& bear::communication::messageable::get_name() const
{
  return m_name;
} // messageable::get_name()

/*---------------------------------------------------------------------------*/
/**
 * \brief Add a message in the message queue.
 * \param msg The message to enqueue.
 */
void bear::communication::messageable::post_message(message& msg)
{
  message* m = &msg;
  register_item( m );
} // messageable::post_message()

/*---------------------------------------------------------------------------*/
/**
 * \brief Immediatly process a message.
 * \param msg The message to process.
 * \return true if the message has been proceded.
 */
bool bear::communication::messageable::send_message(message& msg)
{
  return process_message( msg );
} // messageable::send_message()

/*---------------------------------------------------------------------------*/
/**
 * \brief Process all messages in queue.
 */
void bear::communication::messageable::process_messages()
{
  CLAW_PRECOND( !locked() );

  std::list<message*>::iterator it;

  lock();

  for (it=m_message_queue.begin(); it!=m_message_queue.end(); ++it)
    process_message(**it);

  m_message_queue.clear();

  unlock();
} // messageable::process_messages()

/*---------------------------------------------------------------------------*/
/**
 * \brief Process one message.
 * \remark You should specialize this method.
 * \return true if the message has been proceded.
 */
bool bear::communication::messageable::process_message(message& msg)
{
  return msg.apply_to(*this);
} // messageable::process_message()

/*---------------------------------------------------------------------------*/
/**
 * \brief Enqueue a message.
 * \param who The message to add.
 */
void bear::communication::messageable::add( message* const& who )
{
  m_message_queue.push_back( who );
} // messageable::messageable::add()

/*---------------------------------------------------------------------------*/
/**
 * \brief Should remove a message from the queue but do nothing.
 * \param who The message that should be removed.
 * \remark This method shouldn't be called.
 */
void bear::communication::messageable::remove( message* const& who )
{
  claw::logger << claw::log_warning
               << "messageable::remove(): souldn't be called." << std::endl;
} // messageable::remove()
