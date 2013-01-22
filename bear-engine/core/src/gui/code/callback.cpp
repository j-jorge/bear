/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::gui::callback class.
 * \author Julien Jorge
 */
#include "gui/callback.hpp"

#include "gui/base_callback.hpp"

#include <algorithm>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::gui::callback::callback()
  : m_callback(NULL)
{

} // callback::callback()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param c The callback to execute.
 */
bear::gui::callback::callback( const base_callback& c )
  : m_callback(c.clone())
{

} // callback::callback()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bear::gui::callback::callback( const callback& that )
  : m_callback(NULL)
{
  if ( that.m_callback != NULL )
    m_callback = that.m_callback->clone();
} // callback::callback()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::gui::callback::~callback()
{
  delete m_callback;
} // callback::~callback()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignement operator.
 * \param that The instance to copy from.
 */
bear::gui::callback& bear::gui::callback::operator=( const callback& that )
{
  callback tmp(that);
  std::swap(tmp.m_callback, m_callback);
  return *this;
} // callback::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute the callback.
 */
void bear::gui::callback::execute()
{
  if ( m_callback != NULL )
    m_callback->execute();
  else
    claw::logger << claw::log_warning << "Executing a NULL callback."
                 << std::endl;
} // callback::execute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check for equality.
 * \param that The instance to compare to.
 */
bool bear::gui::callback::operator==( const callback& that ) const
{
  return m_callback == that.m_callback;
} // callback::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check for disequality.
 * \param that The instance to compare to.
 */
bool bear::gui::callback::operator!=( const callback& that ) const
{
  return m_callback != that.m_callback;
} // callback::operator!=()
