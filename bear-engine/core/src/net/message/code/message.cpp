/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::net::message class.
 * \author Julien Jorge
 */
#include "net/message/message.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Print a formatted message in a stream.
 * \param The stream in which the message is written.
 * \param mThe message to write.
 */
std::ostream& operator<<( std::ostream& os, const bear::net::message& m )
{
  return m.formatted_output(os);
} // operator<<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a formatted message from a stream.
 * \param is The stream from which the message is read.
 * \param m (out) The message read from the stream.
 */
std::istream& operator>>( std::istream& is, bear::net::message& m )
{
  return m.formatted_input(is);
} // operator>>()

/*----------------------------------------------------------------------------*/
/**
 * Constructor.
 */
bear::net::message::message()
  : m_date(0)
{
  // nothing to do
} // message::message()

/*----------------------------------------------------------------------------*/
/**
 * Destructor.
 */
bear::net::message::~message()
{
  // nothing to do
} // message::~message()

/*----------------------------------------------------------------------------*/
/**
 * Get the name of this kind of messages.
 */
std::string bear::net::message::get_name() const
{
  return do_get_name();
} // message::get_name()

/*----------------------------------------------------------------------------*/
/**
 * Get the date at which the message must be processed.
 */
std::size_t bear::net::message::get_date() const
{
  return m_date;
} // message::get_date()

/*----------------------------------------------------------------------------*/
/**
 * Set the date at which the message must be processed.
 * \param date The date
 */
void bear::net::message::set_date( std::size_t date )
{
  m_date = date;
} // message::set_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a formatted representation of this message in a stream.
 * \param os The stream in which we write.
 */
std::ostream& bear::net::message::formatted_output( std::ostream& os ) const
{
  return os;
} // message::formatted_output()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a formatted representation of this message from a stream.
 * \param os The stream from which we read.
 */
std::istream& bear::net::message::formatted_input( std::istream& is )
{
  return is;
} // message::formatted_input()
