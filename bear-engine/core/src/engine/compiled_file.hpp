/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class masks the kind of input (text or binary) to the level
 *        loader.
 * \author Julien Jorge
 */
#ifndef __ENGINE_COMPILED_FILE_HPP__
#define __ENGINE_COMPILED_FILE_HPP__

#include <iostream>

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief This class masks the kind of input (text or binary) to the level
     *        loader.
     */
    class ENGINE_EXPORT compiled_file
    {
    public:
      compiled_file( std::istream& f, bool text );

      compiled_file& operator>>( std::string& s );
      compiled_file& operator>>( unsigned long& i );
      compiled_file& operator>>( long& i );
      compiled_file& operator>>( unsigned int& i );
      compiled_file& operator>>( int& i );
      compiled_file& operator>>( double& i );
      compiled_file& operator>>( bool& b );

      operator bool() const;

    private:
      void input_string_as_binary( std::string& s );
      void input_string_as_text( std::string& s );

      void input_long_as_binary( long& i );
      void input_long_as_text( long& i );

      void input_unsigned_long_as_binary( unsigned long& i );
      void input_unsigned_long_as_text( unsigned long& i );

      void input_integer_as_binary( int& i );
      void input_integer_as_text( int& i );

      void input_unsigned_integer_as_binary( unsigned int& i );
      void input_unsigned_integer_as_text( unsigned int& i );

      void input_real_as_binary( double& r );
      void input_real_as_text( double& r );

      void input_bool_as_binary( bool& b );
      void input_bool_as_text( bool& b );

    private:
      /** \brief The file we are writing in. */
      std::istream& m_file;

      /** \brief Are we in text mode ? */
      bool m_text;

    }; // compiled_file
  } // namespace engine
} // namespace bear

#endif // __ENGINE_COMPILED_FILE_HPP__
