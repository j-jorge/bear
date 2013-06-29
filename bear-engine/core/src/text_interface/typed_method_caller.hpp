/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Base class for calling a method of an instance given the string
 *        representation of the value of the arguments. Contrary to
 *        method_caller, this class cast the instance to a given type.
 * \author Julien Jorge.
 */
#ifndef __TEXT_INTERFACE_TYPED_METHOD_CALLER_HPP__
#define __TEXT_INTERFACE_TYPED_METHOD_CALLER_HPP__

#include <vector>
#include <string>

#include "text_interface/method_caller.hpp"

namespace bear
{
  namespace text_interface
  {
    /**
     * \brief Base class for calling a method of an instance given the string
     *        representation of the value of the arguments. Contrary to
     *        method_caller, this class cast the instance to a given type.
     *
     * \author Julien Jorge.
     */
    template<typename SelfClass>
    class typed_method_caller:
      public method_caller
    {
    public:
      /**
       * \brief Execute a method of a given instance.
       * \param self The instance on which the method is called.
       * \param args The string representation of the value of the arguments
       *        passed to the method.
       * \param c The converter used to convert the arguments.
       */
      virtual void explicit_execute
      ( SelfClass& self, const std::vector<std::string>& args,
        const argument_converter& c ) const = 0;

    private:
      void execute
      ( base_exportable* self, const std::vector<std::string>& args,
        const argument_converter& c ) const;

    }; // class typed_method_caller

  } // namespace text_interface
} // namespace bear

#include "text_interface/impl/typed_method_caller.tpp"

#endif // __TEXT_INTERFACE_TYPED_METHOD_CALLER_HPP__
