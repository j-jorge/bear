/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Converts a type into its string representation as used for the
 *        serialisation of variables.
 * \author Julien Jorge
 */
#ifndef __ENGINE_TYPE_TO_STRING_HPP__
#define __ENGINE_TYPE_TO_STRING_HPP__

#include <string>

namespace bear
{
  namespace engine
  {
    /**
     * \brief Converts a type into its string representation as used for the
     *        serialisation of variables.
     * \author Julien Jorge
     */
    template<typename T>
    class type_to_string;

    /**
     * \brief Specialization for type int.
     * \author Julien Jorge
     */
    template<>
    class type_to_string<int>
    {
    public:
      /** \brief The string representation of the type. */
      static const char* const value;

    }; // class type_to_string [int]

    /**
     * \brief Specialization for type unsigned int.
     * \author Julien Jorge
     */
    template<>
    class type_to_string<unsigned int>
    {
    public:
      /** \brief The string representation of the type. */
      static const char* const value;

    }; // class type_to_string [unsigned int]

    /**
     * \brief Specialization for type bool.
     * \author Julien Jorge
     */
    template<>
    class type_to_string<bool>
    {
    public:
      /** \brief The string representation of the type. */
      static const char* const value;

    }; // class type_to_string [bool]

    /**
     * \brief Specialization for type std::string.
     * \author Julien Jorge
     */
    template<>
    class type_to_string<std::string>
    {
    public:
      /** \brief The string representation of the type. */
      static const char* const value;

    }; // class type_to_string []

    /**
     * \brief Specialization for type double.
     * \author Julien Jorge
     */
    template<>
    class type_to_string<double>
    {
    public:
      /** \brief The string representation of the type. */
      static const char* const value;

    }; // class type_to_string [double]

  } // namespace engine
} // namespace bear

#endif // __ENGINE_TYPE_TO_STRING_HPP__
