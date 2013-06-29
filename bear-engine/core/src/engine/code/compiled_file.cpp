/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::compiled_file class.
 * \author Julien Jorge
 */
#include <limits>
#include <climits>

#include "engine/compiled_file.hpp"

#include <claw/string_algorithm.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The file from which we will read.
 * \param text True if we use the file as a text file, false for binary.
 */
bear::engine::compiled_file::compiled_file( std::istream& f, bool text )
  : m_file(f), m_text(text)
{

} // compiled_file::compiled_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a string from the file.
 * \param s The string to read.
 */
bear::engine::compiled_file&
bear::engine::compiled_file::operator>>( std::string& s )
{
  if (m_text)
    input_string_as_text(s);
  else
    input_string_as_binary(s);

  return *this;
} // compiled_file::operator>>() [string]

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an long from the file.
 * \param i The long to read.
 */
bear::engine::compiled_file& bear::engine::compiled_file::operator>>( long& i )
{
  if (m_text)
    input_long_as_text(i);
  else
    input_long_as_binary(i);

  return *this;
} // compiled_file::operator>>() [long]

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an unsigned long from the file.
 * \param i The long to read.
 */
bear::engine::compiled_file&
bear::engine::compiled_file::operator>>( unsigned long& i )
{
  if (m_text)
    input_unsigned_long_as_text(i);
  else
    input_unsigned_long_as_binary(i);

  return *this;
} // compiled_file::operator>>() [unsigned long]

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an integer from the file.
 * \param i The integer to read.
 */
bear::engine::compiled_file& bear::engine::compiled_file::operator>>( int& i )
{
  if (m_text)
    input_integer_as_text(i);
  else
    input_integer_as_binary(i);

  return *this;
} // compiled_file::operator>>() [int]

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an unsigned integer from the file.
 * \param i The integer to read.
 */
bear::engine::compiled_file&
bear::engine::compiled_file::operator>>( unsigned int& i )
{
  if (m_text)
    input_unsigned_integer_as_text(i);
  else
    input_unsigned_integer_as_binary(i);

  return *this;
} // compiled_file::operator>>() [unsigned int]

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a real from the file.
 * \param r The value to read.
 */
bear::engine::compiled_file&
bear::engine::compiled_file::operator>>( double& r )
{
  if (m_text)
    input_real_as_text(r);
  else
    input_real_as_binary(r);

  return *this;
} // compiled_file::operator>>() [real]

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a boolean from the file.
 * \param b The value to read.
 */
bear::engine::compiled_file& bear::engine::compiled_file::operator>>( bool& b )
{
  if (m_text)
    input_bool_as_text(b);
  else
    input_bool_as_binary(b);

  return *this;
} // compiled_file::operator>>() [bool]

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert to a boolean indicating if the stream is ok.
 */
bear::engine::compiled_file::operator bool() const
{
  return !!m_file;
} // compiled_file::operator bool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a string from the file.
 * \param s The string to read.
 */
void bear::engine::compiled_file::input_string_as_binary( std::string& s )
{
  unsigned int len;

  m_file.read( reinterpret_cast<char*>(&len), sizeof(len) );

  std::string::value_type* buffer = new char[len+1];

  m_file.read( buffer, sizeof(std::string::value_type) * len );

  buffer[len] = '\0';
  s = buffer;

  delete[] buffer;
} // compiled_file::input_string_as_binary()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a string from the file.
 * \param s The string to read.
 */
void bear::engine::compiled_file::input_string_as_text( std::string& s )
{
  claw::text::getline(m_file, s);
} // compiled_file::input_string_as_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an long from the file.
 * \param i The long to read.
 */
void bear::engine::compiled_file::input_long_as_binary( long& i )
{
  m_file.read( reinterpret_cast<char*>(&i), sizeof(i) );
} // compiled_file::input_long_as_binary()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an long from the file.
 * \param i The long to read.
 */
void bear::engine::compiled_file::input_long_as_text( long& i )
{
  m_file >> i;
  m_file.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
} // compiled_file::input_long_as_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an unsigned long from the file.
 * \param i The long to read.
 */
void
bear::engine::compiled_file::input_unsigned_long_as_binary( unsigned long& i )
{
  m_file.read( reinterpret_cast<char*>(&i), sizeof(i) );
} // compiled_file::input_unsigned_long_as_binary()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an unsigned long from the file.
 * \param i The long to read.
 */
void
bear::engine::compiled_file::input_unsigned_long_as_text( unsigned long& i )
{
  m_file >> i;
  m_file.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
} // compiled_file::input_unsigned_long_as_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an integer from the file.
 * \param i The integer to read.
 */
void bear::engine::compiled_file::input_integer_as_binary( int& i )
{
  m_file.read( reinterpret_cast<char*>(&i), sizeof(i) );
} // compiled_file::input_integer_as_binary()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an integer from the file.
 * \param i The integer to read.
 */
void bear::engine::compiled_file::input_integer_as_text( int& i )
{
  m_file >> i;
  m_file.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
} // compiled_file::input_integer_as_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an unsigned integer from the file.
 * \param i The integer to read.
 */
void
bear::engine::compiled_file::input_unsigned_integer_as_binary( unsigned int& i )
{
  m_file.read( reinterpret_cast<char*>(&i), sizeof(i) );
} // compiled_file::input_unsigned_integer_as_binary()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an unsigned integer from the file.
 * \param i The integer to read.
 */
void
 bear::engine::compiled_file::input_unsigned_integer_as_text( unsigned int& i )
{
  m_file >> i;
  m_file.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
} // compiled_file::input_unsigned_integer_as_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a real from the file.
 * \param r The real to read.
 */
void bear::engine::compiled_file::input_real_as_binary( double& r )
{
  m_file.read( reinterpret_cast<char*>(&r), sizeof(r) );
} // compiled_file::input_real_as_binary()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a real from the file.
 * \param r The real to read.
 */
void bear::engine::compiled_file::input_real_as_text( double& r )
{
  m_file >> r;
  m_file.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
} // compiled_file::input_real_as_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a boolean from the file.
 * \param b The boolean to read.
 */
void bear::engine::compiled_file::input_bool_as_binary( bool& b )
{
  m_file.read( reinterpret_cast<char*>(&b), sizeof(b) );
} // compiled_file::input_bool_as_binary()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a bool from the file.
 * \param b The bool to read.
 */
void bear::engine::compiled_file::input_bool_as_text( bool& b )
{
  m_file >> b;
  m_file.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
} // compiled_file::input_bool_as_text()
