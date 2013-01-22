/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::check_error class.
 * \author Julien Jorge
 */
#include "bf/check_error.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param msg A description of the error.
 */
bf::check_error::check_error( const std::string& msg )
  : m_message(msg)
{

} // check_error::check_error()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param cause A description of the element on which the error occured.
 * \param msg A description of the error.
 */
bf::check_error::check_error( const std::string& cause, const std::string& msg )
  : m_cause(cause), m_message(msg)
{

} // check_error::check_error()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the description of the element on which the error occured.
 */
const std::string& bf::check_error::get_cause() const
{
  return m_cause;
} // check_error::get_cause()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the description of the error.
 */
const std::string& bf::check_error::get_message() const
{
  return m_message;
} // check_error::get_message()
