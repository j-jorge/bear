/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Convert a string to the equivalent value to pass as an argument for a
 *        character method.
 * \author Julien Jorge.
 */
#ifndef __TEXT_INTERFACE_STRING_TO_ARG_HPP__
#define __TEXT_INTERFACE_STRING_TO_ARG_HPP__

#include "text_interface/get_inner_type.hpp"

#include <string>
#include <vector>
#include <list>

#include <claw/meta/type_list.hpp>

namespace bear
{
  namespace text_interface
  {
    class argument_converter;

    /**
     * \brief Helper class to decide whether an argument is converted using the
     *        default procedure or delegated to the implementation of the
     *        subclass.
     */
    template<typename T, bool DefaultConversion>
    struct string_to_arg_helper;

    // Do the default conversion of the argument
    template<typename T>
    struct string_to_arg_helper<T, true>
    {
      /** The type of the result value obtained with this converter. */
      typedef typename get_inner_type<T>::type result_type;

      static result_type convert_argument
      ( const argument_converter& c, const std::string& arg );
    }; // struct string_to_arg_helper [true]

    // Let the sub class convert the argument.
    template<typename T>
    struct string_to_arg_helper<T, false>
    {
      /** The type of the result value obtained with this converter. */
      typedef T result_type;

      static T convert_argument
      ( const argument_converter& c, const std::string& arg );
    }; // struct string_to_arg_helper [false]

    // Let the sub class convert the argument.
    template<typename T>
    struct string_to_arg_helper<T&, false>
    {
      /** The type of the result value obtained with this converter. */
      typedef T& result_type;

      static result_type convert_argument
      ( const argument_converter& c, const std::string& arg );
    }; // struct string_to_arg_helper [false]

    // Let the sub class convert the argument.
    template<typename T>
    struct string_to_arg_helper<const T&, false>
    {
      /** The type of the result value obtained with this converter. */
      typedef const T& result_type;

      static result_type convert_argument
      ( const argument_converter& c, const std::string& arg );
    }; // struct string_to_arg_helper [false]

    /**
     * \brief Convert a string to the equivalent value to pass as an argument
     *        for a character method.
     * \author Julien Jorge.
     */
    template<typename T>
    struct string_to_arg:
      public string_to_arg_helper
      <
        T,
        claw::meta::type_list_contains
        <
          T,
          claw::meta::cpp_type_list
        >::result
      >
    { }; // struct string_to_arg

    /**
     * \brief Convert a string to the equivalent value to pass as an argument
     *        for a character method.
     * \author Julien Jorge.
     */
    template<typename T>
    struct string_to_arg<const T&>:
      public string_to_arg_helper
      <
        const T&,
        claw::meta::type_list_contains
        <
          T,
          claw::meta::cpp_type_list
        >::result
      >
    { }; // struct string_to_arg

    /**
     * \brief Specialisation for std::string. There is no conversion to do.
     * \author Sébastien Angibaud
     */
    template<>
    struct string_to_arg<std::string>
    {
      /** The type of the result value obtained with this converter. */
      typedef std::string result_type;

      static std::string convert_argument
      ( const argument_converter& c, const std::string& arg );
    }; // struct string_to_arg [std::string]

    /**
     * \brief Specialisation for std::string&. There is no conversion to do.
     * \author Sébastien Angibaud
     */
    template<>
    struct string_to_arg<std::string&>:
      public string_to_arg<std::string>
    { }; // struct string_to_arg [std::string&]

    /**
     * \brief Specialisation for const std::string&. There is no conversion to
     *        do.
     * \author Sébastien Angibaud
     */
    template<>
    struct string_to_arg<const std::string&>:
      public string_to_arg<std::string>
    { }; // struct string_to_arg [const std::string&]

    /**
     * \brief Convert a string to the equivalent sequence of value to pass as an
     *        argument for a character method.
     * \author Julien Jorge
     */
    template<typename Sequence>
    struct string_to_sequence_arg
    {
    public:
      typedef Sequence result_type;

      static result_type convert_argument
      ( const argument_converter& c, const std::string& arg );
    }; // struct string_to_sequence_arg

    /**
     * \brief Specialisation for std::vector.
     * \author Julien Jorge
     */
    template<typename T>
    struct string_to_arg< const std::vector<T>& >:
      public string_to_sequence_arg< std::vector<T> >
    { };

    /**
     * \brief Specialisation for std::list.
     * \author Julien Jorge
     */
    template<typename T>
    struct string_to_arg< const std::list<T>& >:
      public string_to_sequence_arg< std::list<T> >
    { };

  } // namespace text_interface
} // namespace bear

#include "text_interface/impl/string_to_arg.tpp"

#endif // __TEXT_INTERFACE_STRING_TO_ARG_HPP__
