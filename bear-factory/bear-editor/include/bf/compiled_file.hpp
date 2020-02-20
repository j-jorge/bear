/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class outputs data in a format understood by the game.
 * \author Julien Jorge
 */
#ifndef __BF_COMPILED_FILE_HPP__
#define __BF_COMPILED_FILE_HPP__

#include <iostream>
#include "bf/libeditor_export.hpp"

namespace bf
{
  /**
   * \brief This class outputs data in a format understood by the game.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT compiled_file
  {
  public:
    compiled_file( std::ostream& f );

    compiled_file& operator<<( const std::string& s );
    compiled_file& operator<<( unsigned long i );
    compiled_file& operator<<( long i );
    compiled_file& operator<<( unsigned int i );
    compiled_file& operator<<( int i );
    compiled_file& operator<<( double i );

  private:
    void output_string_as_text( const std::string& s );
    void output_long_as_text( long i );
    void output_unsigned_long_as_text( unsigned long i );
    void output_integer_as_text( int i );
    void output_unsigned_integer_as_text( unsigned int i );
    void output_real_as_text( double r );

  private:
    /** \brief The file we are writing in. */
    std::ostream& m_file;

  }; // compiled_file
} // namespace bf

#endif // __BF_COMPILED_FILE_HPP__
