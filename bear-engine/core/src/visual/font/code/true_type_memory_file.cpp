/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::true_type_memory_file class.
 * \author Julien Jorge
 */
#include "visual/font/true_type_memory_file.hpp"

#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param is The stream from which the file's content is read.
 */
bear::visual::true_type_memory_file::true_type_memory_file( std::istream& is )
{
  is.seekg( 0, std::ios_base::end );
  m_size = is.tellg();

  unsigned char* buffer = new unsigned char[ m_size ];
  is.seekg( 0, std::ios_base::beg );
  is.read( reinterpret_cast<char*>(buffer), m_size );

  m_content = content_pointer_type( buffer );
} // true_type_memory_file::true_type_memory_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns a pointer to the first byte of data.
 */
const unsigned char* bear::visual::true_type_memory_file::begin() const
{
  return m_content.get();
} // true_type_memory_file::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns a pointer to the end of the data.
 */
const unsigned char* bear::visual::true_type_memory_file::end() const
{
  return begin() + m_size;
} // true_type_memory_file::end()
