/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::text_interface::argument_converter class.
 * \author Julien Jorge.
 */
#include "text_interface/argument_converter.hpp"

#include <typeinfo>

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::text_interface::argument_converter::~argument_converter()
{
  // nothing to do
} // argument_converter::~argument_converter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Delegate conversion of an argument, if no adequate conversion has
 *        been found.
 * \param arg The argument to convert.
 * \param type The description of the expected type for the result value.
 */
bear::text_interface::converted_argument
bear::text_interface::argument_converter::do_convert_argument
( const std::string& arg, const std::type_info& type ) const
{
  throw no_converter(arg, type);
} // argument_converter::do_convert_argument()
