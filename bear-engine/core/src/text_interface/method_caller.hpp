/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Base class for calling a method of an instance given the string
 *        representation of the value of the arguments.
 * \author Julien Jorge.
 */
#ifndef __TEXT_INTERFACE_METHOD_CALLER_HPP__
#define __TEXT_INTERFACE_METHOD_CALLER_HPP__

#include <vector>
#include <string>

namespace bear
{
  namespace text_interface
  {
    class base_exportable;
    class argument_converter;

    /**
     * \brief Base class for calling a method of an instance given the string
     *        representation of the value of the arguments.
     *
     * \author Julien Jorge.
     */
    class method_caller
    {
    public:
      /**
       * \brief Execute a method of a given instance.
       * \param self The instance on which the method is called.
       * \param args The string representation of the value of the arguments
       *        passed to the method.
       * \param c The converter used to convert the arguments.
       */
      virtual void execute
      ( base_exportable* self, const std::vector<std::string>& args,
        const argument_converter& c ) const = 0;

    }; // class method_caller

  } // namespace text_interface
} // namespace bear

#endif // __TEXT_INTERFACE_METHOD_CALLER_HPP__
