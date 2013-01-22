/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
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

    bool read_bool
    ( const claw::configuration_file& config,
      const std::string& section, const std::string& field_name,
      bool default_value ) const;

    void write_rect( std::ostream& f, const wxRect& r ) const;

  public:
    /** \brief Position and size of the main window. */
    wxRect main_rect;

    /** \brief Visibility of the item properties window. */
    bool properties_visible;

    /** \brief Visibility of the layer list window. */
    bool layer_list_visible;

    /** \brief Visibility of the item class pool window. */
    bool item_class_pool_visible;

    /** \brief Default position and size for the level windows. */
    wxRect default_level_window_rect;

    /** \brief List of recent used classes. */
    std::list< std::string > m_recent_used_classes;

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

    /** \brief Section of the item properties frame. */
    static const std::string s_properties_frame_section;

    /** \brief Section of the layer list frame. */
    static const std::string s_layer_list_frame_section;

    /** \brief Section of the item class pool frame. */
    static const std::string s_item_class_pool_frame_section;

    /** \brief Section of the level frame. */
    static const std::string s_level_frame_section;

    /** \brief Label of the x-position fields. */
    static const std::string s_x_field;

    /** \brief Label of the y-position fields. */
    static const std::string s_y_field;

    /** \brief Label of the width fields. */
    static const std::string s_width_field;

    /** \brief Label of the height fields. */
    static const std::string s_height_field;

    /** \brief Label of the visibility fields. */
    static const std::string s_visible_field;

  }; // class configuration
} // namespace bf

#endif // __BF_CONFIGURATION_HPP__
