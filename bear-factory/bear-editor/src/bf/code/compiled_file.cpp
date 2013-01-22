/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::compiled_file class.
 * \author Julien Jorge
 */
#include "bf/compiled_file.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The file in wich we will write.
 */
bf::compiled_file::compiled_file( std::ostream& f )
  : m_file(f)
{

} // compiled_file::compiled_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a string in the file.
 * \param s The string to write.
 */
bf::compiled_file& bf::compiled_file::operator<<( const std::string& s )
{
  output_string_as_text(s);

  return *this;
} // compiled_file::operator<<() [string]

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an long in the file.
 * \param i The long to write.
 */
bf::compiled_file& bf::compiled_file::operator<<( long i )
{
  output_long_as_text(i);

  return *this;
} // compiled_file::operator<<() [long]

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an unsigned long in the file.
 * \param i The long to write.
 */
bf::compiled_file& bf::compiled_file::operator<<( unsigned long i )
{
  output_unsigned_long_as_text(i);

  return *this;
} // compiled_file::operator<<() [unsigned long]

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an integer in the file.
 * \param i The integer to write.
 */
bf::compiled_file& bf::compiled_file::operator<<( int i )
{
  output_integer_as_text(i);

  return *this;
} // compiled_file::operator<<() [int]

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an unsigned integer in the file.
 * \param i The integer to write.
 */
bf::compiled_file& bf::compiled_file::operator<<( unsigned int i )
{
  output_unsigned_integer_as_text(i);

  return *this;
} // compiled_file::operator<<() [unsigned int]

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a real in the file.
 * \param r The value to write.
 */
bf::compiled_file& bf::compiled_file::operator<<( double r )
{
  output_real_as_text(r);

  return *this;
} // compiled_file::operator<<() [real]

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a string in the file.
 * \param s The string to write.
 */
void bf::compiled_file::output_string_as_text( const std::string& s )
{
  m_file << s << std::endl;
} // compiled_file::output_string_as_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an long in the file.
 * \param i The long to write.
 */
void bf::compiled_file::output_long_as_text( long i )
{
  m_file << i << std::endl;
} // compiled_file::output_long_as_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an unsigned long in the file.
 * \param i The long to write.
 */
void bf::compiled_file::output_unsigned_long_as_text( unsigned long i )
{
  m_file << i << std::endl;
} // compiled_file::output_unsigned_long_as_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an integer in the file.
 * \param i The integer to write.
 */
void bf::compiled_file::output_integer_as_text( int i )
{
  m_file << i << std::endl;
} // compiled_file::output_integer_as_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an unsigned integer in the file.
 * \param i The integer to write.
 */
void bf::compiled_file::output_unsigned_integer_as_text( unsigned int i )
{
  m_file << i << std::endl;
} // compiled_file::output_unsigned_integer_as_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a real in the file.
 * \param r The real to write.
 */
void bf::compiled_file::output_real_as_text( double r )
{
  m_file << r << std::endl;
} // compiled_file::output_real_as_text()
