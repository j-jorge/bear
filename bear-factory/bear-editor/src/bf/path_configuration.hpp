/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A singleton that stores the paths used in the programs.
 * \author Julien Jorge
 */
#ifndef __BF_PATH_CONFIGURATION_HPP__
#define __BF_PATH_CONFIGURATION_HPP__

#include "bf/libeditor_export.hpp"

#include <string>
#include <list>
#include <claw/basic_singleton.hpp>

namespace bf
{
  /**
   * \brief A singleton that stores the paths used in the programs.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT path_configuration:
    public claw::pattern::basic_singleton<path_configuration>
  {
  private:
    /** \brief The result found by a call to find_random_file_name_on_disk(). */
    struct random_file_result
    {
      random_file_result
      ( const std::string p, std::size_t n, const std::list<std::string>& c );

      /** \brief The pattern used for the search. */
      const std::string pattern;

      /** \brief The maximum number of results asked. */
      const std::size_t count;

      /** \brief The files found in the data directory. */
      const std::list<std::string> candidates;

    }; // struct random_file_result

    /** \brief The type of the container in which we store the results of the
        calls to find_random_file_name_on_disk(). */
    typedef std::list<random_file_result> cached_random_file_list_type;

  public:
    path_configuration();

    void save() const;

    std::string get_config_directory() const;

    bool get_full_path( std::string& p ) const;
    bool expand_file_name( std::string& p ) const;
    bool expand_file_name( std::string& p, std::size_t m ) const;
    bool get_relative_path( std::string& p ) const;

  private:
    void load();

    bool create_config_directory() const;
    bool create_config_file() const;

    bool find_random_file_name( std::string& name, std::size_t m ) const;
    bool find_cached_random_file_name( std::string& name, std::size_t m ) const;
    bool
      find_random_file_name_on_disk( std::string& name, std::size_t m ) const;

    void find_all_files_in_dir
      ( const std::string& dirname, const std::string& pattern,
        std::size_t offset, std::size_t m,
        std::list<std::string>& result ) const;
    bool glob_match
      ( const std::string& pattern, const std::string& text,
        std::size_t offset ) const;
    bool glob_potential_match
      ( const std::string& pattern, const std::string& text,
        std::size_t offset ) const;

  public:
    /** \brief Path to the directory containing the XML files for item
        classes. */
    std::list<std::string> item_class_path;

    /** \brief Path to the data directory of the game. */
    std::list<std::string> data_path;

  private:
    /** \brief Recent results obtained with find_random_file_name. */
    mutable cached_random_file_list_type m_cached_random_file;

    /** \brief The maximum number of entries in m_cached_random_file. */
    std::size_t m_max_cached_files;

    /** \brief The name of the configuration file. */
    static const std::string s_config_file_name;

    /** \brief The path to the directory containing the configuration file,
        relative to the user's directory. */
    static const std::string s_config_directory;

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

    /** \brief The field of the directory of XML files for item classes. */
    static const std::string s_items_directory_field;

    /** \brief The field of the directory of the path of datas. */
    static const std::string s_data_directory_field;

  }; // class path_configuration
} // namespace bf

#endif // __BF_PATH_CONFIGURATION_HPP__
