/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::sync class.
 * \author Julien Jorge
 */
#include "engine/network/message/sync.hpp"

#include <iostream>

MESSAGE_EXPORT( sync, bear::engine )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::sync::sync()
{

} // sync::sync()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param id An identifier associated with the message.
 * \param active_sync Tell if this synchronization is the result of an active
 *        iteration.
 */
bear::engine::sync::sync( std::size_t id, bool active_sync )
  : m_id(id), m_active_sync(active_sync)
{

} // sync::sync()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the identifier associated with the synchronization.
 */
std::size_t bear::engine::sync::get_id() const
{
  return m_id;
} // sync::get_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this synchronization is the result of an active
 *        iteration.
 */
bool bear::engine::sync::is_active_sync() const
{
  return m_active_sync;
} // sync::is_active_sync()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a formatted representation of this message in a stream.
 * \param os The stream in which we write.
 */
std::ostream& bear::engine::sync::formatted_output( std::ostream& os ) const
{
  return os << m_id << ' ' << m_active_sync;
} // sync::formatted_output()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a formatted representation of this message from a stream.
 * \param os The stream from which we read.
 */
std::istream& bear::engine::sync::formatted_input( std::istream& is )
{
  return is >> m_id >> m_active_sync;
} // sync::formatted_input()
