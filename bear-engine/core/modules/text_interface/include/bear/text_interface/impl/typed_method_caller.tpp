/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::text_interface::typed_method_caller class.
 * \author Julien Jorge.
 */

#include <claw/logger/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute a method of a given instance.
 * \param self The instance on which the method is called.
 * \param args The string representation of the value of the arguments passed to
 *        the method.
 * \param c The converter used to convert the arguments.
 */
template<typename SelfClass>
void bear::text_interface::typed_method_caller<SelfClass>::execute
( base_exportable* self, const std::vector<std::string>& args,
  const argument_converter& c )  const
{
  SelfClass* s = dynamic_cast<SelfClass*>(self);

  if ( s!=NULL )
    explicit_execute(*s, args, c);
  else
    claw::logger << claw::log_warning << "Failed to cast base_exportable."
                 << std::endl;
} // typed_method_caller::execute()
