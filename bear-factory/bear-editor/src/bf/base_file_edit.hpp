/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Base control for editing a field whose value is a path to a file.
 * \author Julien Jorge
 */
#ifndef __BF_BASE_FILE_EDIT_HPP__
#define __BF_BASE_FILE_EDIT_HPP__

#include "bf/simple_edit.hpp"

#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/window.h>

namespace bf
{
  class workspace_environment;

  /**
   * \brief Base control for editing a field whose value is a path to a file.
   * \author Julien Jorge
   */
  template<typename T>
  class base_file_edit:
    public simple_edit<T>,
    public wxPanel
  {
  public:
    /** \brief The type of the content of the file. */
    typedef T file_type;

    /** \brief The type of the current class. */
    typedef base_file_edit<T> self_type;

    /** \brief The type of the parent edit. */
    typedef simple_edit<T> super;

    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_BROWSE
      }; // enum control_id

  public:
    base_file_edit
    ( wxWindow& parent, workspace_environment& env, const file_type& v );
    base_file_edit
    ( wxWindow& parent, workspace_environment& env, const wxString& filter, 
      const file_type& v );

    bool validate();

    void set_filter( wxString filter );

  private:
    void value_updated();

    void create_controls();

    void on_browse(wxCommandEvent& event);

  private:
    /** \brief The filter of the file selection dialog. */
    wxString m_filter;

    /** \brief Text control in which we display the path to the file. */
    wxTextCtrl* m_path;

    /** \brief A button that opens the file selection dialog. */
    wxButton* m_browse;
    
    /** \brief The workspace name. */
    workspace_environment& m_workspace;

  }; // class base_file_edit
} // namespace bf

#include "bf/impl/base_file_edit.tpp"

#endif // __BF_BASE_FILE_EDIT_HPP__
