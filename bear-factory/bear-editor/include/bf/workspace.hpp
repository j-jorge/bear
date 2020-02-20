/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A workspace.
 * \author Sebastien Angibaud
 */
#ifndef __BF_WORKSPACE_HPP__
#define __BF_WORKSPACE_HPP__

#include <string>
#include <list>

#include "bf/libeditor_export.hpp"

namespace bf
{
  /**
   * \brief A workspace.
   * \author Sebastien Angibaud
   */
  class BEAR_EDITOR_EXPORT workspace
  {
  public:
    typedef std::list<std::string> path_list;
    typedef path_list::const_iterator path_list_const_iterator;
    typedef path_list::const_reverse_iterator path_list_const_reverse_iterator;

  public:
    workspace();

    void set_run_path(const std::string& path);
    std::string get_run_path() const;

    void add_item_class_path( const std::string& path );
    void clear_item_class_path();
    void add_data_path( const std::string& path );
    void clear_data_path();
    void clear();
    const path_list& get_item_class_path() const; 

    path_list_const_iterator item_class_begin() const;
    path_list_const_iterator item_class_end() const;

    path_list_const_iterator data_begin() const;
    path_list_const_iterator data_end() const;

    path_list_const_reverse_iterator data_rbegin() const;
    path_list_const_reverse_iterator data_rend() const;

  private:
    /** \brief Path to the directory containing the XML files for item
        classes. */
    path_list m_item_class_path;

    /** \brief Path to the data directory of the game. */
    path_list m_data_path;

    /** \brier The path of the run configuration. */
    std::string m_run_path;
  }; // class workspace
} // namespace bf

#endif // __BF_WORKSPACE_HPP__
