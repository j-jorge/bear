/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Conversion between a value and a text in a stream.
 * \author Julien Jorge
 */
#ifndef __BF_STREAM_CONV_HPP__
#define __BF_STREAM_CONV_HPP__

#include "bf/color.hpp"
#include "bf/custom_type.hpp"
#include "bf/libeditor_export.hpp"

#include <iostream>

namespace bf
{
  /**
   * \brief Conversion between a value and a text in a stream. Not defined for
   *        types that are not based on custom_type.
   * \author Julien Jorge
   */
  template<typename Type>
  class stream_conv
  {
  public:
    typedef Type value_type;

  public:
    static std::istream& read( std::istream& is, value_type& v );

  }; // class stream_conv

  /**
   * \brief Conversion between a value and a text in a stream. Specialisation
   *        for string_type.
   * \author Julien Jorge
   */
  template<>
  class BEAR_EDITOR_EXPORT stream_conv<string_type>
  {
  public:
    typedef string_type value_type;

  public:
    static std::istream& read( std::istream& is, value_type& v );

  }; // class stream_conv [string_type]

  /**
   * \brief Conversion between a value and a text in a stream. Specialisation
   *        for custom_type<bool>.
   * \author Julien Jorge
   */
  template<>
  class BEAR_EDITOR_EXPORT stream_conv< custom_type<bool> >
  {
  public:
    typedef custom_type<bool> value_type;

  public:
    static std::istream& read( std::istream& is, value_type& v );

  }; // class stream_conv [custom_type<bool>]

  /**
   * \brief Conversion between a value and a text in a stream. Specialisation
   *        for custom_type<color>.
   * \author Sébastien Angibaud
   */
  template<>
  class BEAR_EDITOR_EXPORT stream_conv< color >
  {
  public:
    typedef color value_type;

  public:
    static std::istream& read( std::istream& is, value_type& v );

  }; // class stream_conv [custom_type<color>]

  /**
   * \brief Conversion between a value and a text in a stream. Specialisation
   *        for others custom types.
   * \author Julien Jorge
   */
  template<typename T>
  class stream_conv< custom_type<T> >
  {
  public:
    typedef custom_type<T> value_type;

  public:
    static std::istream& read( std::istream& is, value_type& v );

  }; // class stream_conv [custom_type]

  /**
   * \brief Conversion between a value and a text in a stream. Specialisation
   *        for item_reference_type.
   * \author Julien Jorge
   */
  template<>
  class BEAR_EDITOR_EXPORT stream_conv<item_reference_type>:
    public stream_conv< custom_type<std::string> >
  {

  }; // class stream_conv [item_reference_type]

} // namespace bf

#include "bf/impl/stream_conv.tpp"

#endif // __BF_STREAM_CONV_HPP__
