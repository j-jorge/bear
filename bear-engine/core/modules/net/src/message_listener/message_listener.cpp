/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::net::message_listener class.
 * \author Julien Jorge
 */
#include "bear/net/message_listener/message_listener.hpp"

#include "bear/net/message_listener/base_message_listener.hpp"

#include <algorithm>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::net::message_listener::message_listener()
  : m_impl( NULL )
{

} // message_listener::message_listener()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bear::net::message_listener::message_listener( const message_listener& that )
  : m_impl( that.m_impl->clone() )
{

} // message_listener::message_listener()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param listener The implementation of this instance.
 */
bear::net::message_listener::message_listener
( const base_message_listener& listener )
  : m_impl(listener.clone())
{

} // message_listener::message_listener()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::net::message_listener::~message_listener()
{
  delete m_impl;
} // message_listener::~message_listener()

/*----------------------------------------------------------------------------*/
/**
 * \brief Process a message.
 * \param m The message to process.
 */
void bear::net::message_listener::process_message( const message& m ) const
{
  if ( m_impl != NULL )
    m_impl->process_message(m);
} // message_listener::process_message()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assign an other listener to this listener.
 * \param that The other listener.
 */
bear::net::message_listener&
bear::net::message_listener::operator=( const message_listener& that )
{
  message_listener tmp(that);
  swap( tmp );
  return *this;
} // message_listener::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap this listener with an other.
 * \param that The other listener.
 */
void bear::net::message_listener::swap( message_listener& that )
{
  std::swap(m_impl, that.m_impl);
} // message_listener::swap()
