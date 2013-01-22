/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::text_interface::converted_argument class.
 * \author Julien Jorge.
 */
#include "text_interface/converted_argument.hpp"

#include <cstdlib>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::text_interface::converted_argument::converted_argument()
  : m_holder(NULL)
{

} // converted_argument::converted_argument()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param that The instance to copy from.
 */
bear::text_interface::converted_argument::converted_argument
( const converted_argument& that )
  : m_holder(that.m_holder)
{

} // converted_argument::converted_argument()
