/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The paths to the directories where the files are searched and
 *        generated.
 * \author Julien Jorge
 */
#ifndef __SDC_WORKING_DIRECTORY_HPP__
#define __SDC_WORKING_DIRECTORY_HPP__

#include <string>

namespace sdc
{
  /**
   * \brief The paths to the directories where the files are searched and
   *        generated.
   * \author Julien Jorge
   */
  class working_directory
  {
  public:
    explicit working_directory( std::string input_file_name );

    std::string get_input_file_name() const;
    std::string get_xcf_path( std::string xcf_name ) const;
    std::string get_output_image_path( std::string sheet_name ) const;
    std::string get_output_spritepos_path( std::string sheet_name ) const;

  private:
    /** \brief The file on which we work. */
    const std::string m_input_file_name;

    /** The directory where the files are searched. */
    std::string m_input_directory;

    /** The directory where the files are written. */
    std::string m_output_directory;

  }; // working_directory()

} // namespace sdc

#endif // __SDC_WORKING_DIRECTORY_HPP__
