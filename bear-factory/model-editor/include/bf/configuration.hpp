/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The configuration of our program.
 * \author Julien Jorge
 */
#ifndef __BF_CONFIGURATION_HPP__
#define __BF_CONFIGURATION_HPP__

#include <string>
#include <list>
#include <claw/configuration_file.hpp>
#include <wx/gdicmn.h>

namespace bf
{
  /**
   * \brief The configuration of our program.
   * \author Julien Jorge
   */
  class configuration
  {
  public:
    configuration();

    void load();
    void save() const;

  private:
    bool create_config_file() const;

    wxRect read_rect( const claw::configuration_file& config,
                      const std::string& section ) const;

    void write_rect( std::ostream& f, const wxRect& r ) const;

  public:
    /** \brief Position and size of the main window. */
    wxRect main_rect;

    /** \brief Default position and size for the model windows. */
    wxRect default_model_window_rect;

  private:
    /** \brief The name of the configuration file. */
    static const std::string s_config_file_name;

    /** \brief Character written on the left of a section name in the
        configuration file. */
    static const char s_section_left;

    /** \brief Character written on the right of a section name in the
        configuration file. */
    static const char s_section_right;

    /** \brief Character used for comments in the configuration file. */
    static const char s_comment;

    /** \brief Character used for assigning a value to a field. */
    static const char s_field_assign;

    /** \brief Section of the main frame. */
    static const std::string s_main_frame_section;

    /** \brief Section of the model frame. */
    static const std::string s_model_frame_section;

    /** \brief Label of the x-position fields. */
    static const std::string s_x_field;

    /** \brief Label of the y-position fields. */
    static const std::string s_y_field;

    /** \brief Label of the width fields. */
    static const std::string s_width_field;

    /** \brief Label of the height fields. */
    static const std::string s_height_field;

  }; // class configuration
} // namespace bf

#endif // __BF_CONFIGURATION_HPP__
