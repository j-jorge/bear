/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::communication::post_office class.
 * \author Julien Jorge
 */
#include "communication/post_office.hpp"

#include "communication/message.hpp"
#include "communication/messageable.hpp"

#include <claw/logger.hpp>
#include <claw/assert.hpp>

/*---------------------------------------------------------------------------*/
const std::string bear::communication::post_office::no_name;

/*---------------------------------------------------------------------------*/
/**
 * \brief Immediately send a message to an item.
 * \param target The name of the item receiving the message.
 * \param msg The message to send.
 * \return true if the message has been proceded.
 */
bool bear::communication::post_office::send_message
( const std::string& target, message& msg ) const
{
  CLAW_PRECOND( target != no_name );

  std::map<std::string, messageable*>::const_iterator it;
  bool result = false;

  it = m_items.find( target );

  if ( it!=m_items.end() )
    result = it->second->send_message( msg );
  else
    claw::logger << claw::log_warning
                 << "post_office::send_message(): can't find target " << target
                 << std::endl;

  return result;
} // post_office::send_message()

/*---------------------------------------------------------------------------*/
/**
 * \brief Process messages of all items.
 */
void bear::communication::post_office::process_messages()
{
  CLAW_PRECOND( !locked() );

  std::map<std::string, messageable*>::const_iterator it;

  lock();

  for(it=m_items.begin(); it!=m_items.end(); ++it)
    it->second->process_messages();

  unlock();
} // post_office::process_messages()

/*---------------------------------------------------------------------------*/
/**
 * \brief Tell if there exists an item having a given name.
 */
bool bear::communication::post_office::exists( const std::string& name ) const
{
  return m_items.find(name) != m_items.end();
} // post_office::exists()

/*---------------------------------------------------------------------------*/
/**
 * \brief Remove all items.
 */
void bear::communication::post_office::clear()
{
  std::map<std::string, messageable*>::const_iterator it;

  lock();

  for(it=m_items.begin(); it!=m_items.end(); ++it)
    release_item(it->second);

  unlock();
} // post_office::process_messages()

/*---------------------------------------------------------------------------*/
/**
 * \brief Add an item in the list.
 * \param who The item to add.
 */
void bear::communication::post_office::add( messageable* const& who)
{
  if ( who->get_name() == no_name )
    {
      claw::logger << claw::log_warning
                   << "post_office::send_message(): '" << no_name
                   << "' can't be use for a name." << std::endl;
      return;
    }

  std::map<std::string, messageable*>::const_iterator it;

  it = m_items.find( who->get_name() );

  if ( it == m_items.end() )
    m_items[who->get_name()] = who;
  else
    claw::logger << claw::log_warning << "post_office::add(): item "
                 << who->get_name() << " is already in the list" << std::endl;
} // post_office::add()

/*---------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the list.
 * \param who The item to remove.
 */
void bear::communication::post_office::remove(messageable* const& who)
{
  std::map<std::string, messageable*>::iterator it;

  it = m_items.find( who->get_name() );

  if ( it != m_items.end() )
    m_items.erase(it);
  else
    claw::logger << claw::log_warning << "post_office::remove(): item "
                 << who->get_name() << " isn't in the list" << std::endl;
} // post_office::remove()

