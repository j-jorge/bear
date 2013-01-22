/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::item_check_result class.
 * \author Julien Jorge
 */
#include "bf/item_check_result.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an error in the result.
 * \param e The error to add.
 */
void bf::item_check_result::add( const check_error& e )
{
  m_errors.push_back(e);
} // item_check_result::add()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is no error in the result.
 */
bool bf::item_check_result::is_ok() const
{
  return m_errors.empty();
} // item_check_result::is_ok()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the errors.
 */
bf::item_check_result::const_iterator bf::item_check_result::begin() const
{
  return m_errors.begin();
} // item_check_result::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the end of the errors.
 */
bf::item_check_result::const_iterator bf::item_check_result::end() const
{
  return m_errors.end();
} // item_check_result::end()
