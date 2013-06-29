/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::net::base_message_listener class.
 * \author Julien Jorge
 */
#include "net/message_listener/base_message_listener.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::net::base_message_listener::~base_message_listener()
{

} // base_message_listener::~base_message_listener()

/*----------------------------------------------------------------------------*/
/**
 * \brief Process a message.
 * \param m The message to process.
 */
void bear::net::base_message_listener::process_message( const message& m ) const
{
  do_process_message(m);
} // base_message_listener::process_message()
