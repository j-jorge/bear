/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the sdc::working_directory class.
 * \author Julien Jorge
 */
#include "working_directory.hpp"

#include <boost/filesystem/convenience.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param input_file_name The file on which we work.
 */
sdc::working_directory::working_directory( std::string input_file_name )
  : m_input_file_name( input_file_name )
{
  const boost::filesystem::path input_path
    ( m_input_file_name, boost::filesystem::native );
  const boost::filesystem::path input_directory( input_path.parent_path() );

  m_input_directory = input_directory.string();
  m_output_directory = m_input_directory;
} // working_directory::working_directory()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the name of the file on which we work.
 */
std::string sdc::working_directory::get_input_file_name() const
{
  return m_input_file_name;
} // working_directory::get_input_file_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the path to a XCF file.
 * \param xcf_name The name of the file of which we want the path.
 */
std::string sdc::working_directory::get_xcf_path( std::string xcf_name ) const
{
  return m_input_directory + '/' + xcf_name;
} // working_directory::get_xcf_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the path to an image file generated with the input file.
 * \param sheet_name The name of the sprite sheet that generates the image.
 */
std::string
sdc::working_directory::get_output_image_path( std::string sheet_name ) const
{
  return m_output_directory + '/' + sheet_name + ".png";
} // working_directory::get_output_image_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the path to spritepos file generated with the input file.
 * \param sheet_name The name of the sprite sheet that generates the image.
 */
std::string
sdc::working_directory::get_output_spritepos_path
( std::string sheet_name ) const
{
  return m_output_directory + '/' + sheet_name + ".spritepos";
} // working_directory::get_output_spritepos_path()
