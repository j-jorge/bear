/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class that converts a string into the adequate value to be passed
 *        to a function of a base_exportable by the method_caller classes.
 * \author Julien Jorge.
 */
#ifndef __TEXT_INTERFACE_ARGUMENT_CONVERTER_HPP__
#define __TEXT_INTERFACE_ARGUMENT_CONVERTER_HPP__

#include <string>
#include <typeinfo>
#include <claw/exception.hpp>

#include "bear/text_interface/converted_argument.hpp"

namespace bear
{
  namespace text_interface
  {
    template<typename T>
    struct string_to_arg;

    /**
     * \brief This exception is thrown when an argument could not be converted
     *        into the desired type.
     * \author Julien Jorge
     */
    class no_converter:
      public claw::exception
    {
    public:
      no_converter( const std::string& arg, const std::type_info& type )
        : claw::exception
          ( "Can't convert arg '" + arg + "' as '" + type.name() + "'" )
      { }
    }; // class no_converter

    /**
     * \brief A class that converts a string into the adequate value to be
     *        passed to a function of a base_exportable by the method_caller
     *        classes.
     * \author Julien Jorge.
     */
    class argument_converter
    {
    public:
      /** The type of the result of the conversion for a given input type. */
      template<typename T>
      struct conversion_result
      {
        typedef typename string_to_arg<T>::result_type result_type;
      }; // struct conversion_result

    public:
      virtual ~argument_converter();

      template<typename T>
      typename conversion_result<T>::result_type
      convert_argument( const std::string& arg ) const;

      virtual converted_argument do_convert_argument
      ( const std::string& arg, const std::type_info& type ) const;

    }; // class argument_converter

  } // namespace text_interface
} // namespace bear

#include "bear/text_interface/impl/argument_converter.tpp"

#endif // __TEXT_INTERFACE_ARGUMENT_CONVERTER_HPP__
