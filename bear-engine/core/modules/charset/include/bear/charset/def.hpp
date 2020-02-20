/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Various types to use for strings and unicode.
 * \author Julien Jorge
 */
#ifndef __CHARSET_DEF_HPP__
#define __CHARSET_DEF_HPP__

#include <string>
#include <cstdlib>

namespace bear
{
  namespace charset
  {
    /** \brief Unicode character type. */
    typedef wchar_t char_type;

    /** \brief Unicode string. */
    typedef std::wstring string_type;

    /**
     * \brief Converts a given multibyte string into a charset::string_type.
     * \param s The string to convert.
     */
    inline string_type convert_string( const std::string& s )
    {
      // This buffer will receive the result of the conversion of input_text to
      // a wide char string. The size is computed under the assumption that
      // Unicode allows up to 4 bytes per character.
      char_type* const wide_text( new char_type[ 4 * s.size() + 1 ] );
      const std::size_t string_length
        ( std::mbstowcs( wide_text, s.c_str(), s.size() ) );
      
      string_type result;

      if ( string_length != std::size_t( -1 ) )
        result = string_type( wide_text, wide_text + string_length );

      delete[] wide_text;

      return result;
    }
  } // namespace charset
} // namespace bear

#endif // __CHARSET_DEF_HPP__
