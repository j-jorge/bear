/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A dialog to configure the execution of the levels.
 * \author Julien Jorge
 */
#ifndef __BF_RUN_CONFIGURATION_FRAME_HPP__
#define __BF_RUN_CONFIGURATION_FRAME_HPP__

#include "bf/run_configuration.hpp"

#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>

namespace bf
{
  /**
   * \brief A dialog to configure the execution of the levels.
   * \author Julien Jorge
   */
  class run_configuration_frame:
    public wxDialog
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        ID_BROWSE,
        ID_ARGUMENTS,
        ID_ADD_ARGUMENT,
        ID_REMOVE_ARGUMENT
      }; // enum control_id

  public:
    run_configuration_frame( wxWindow* parent );

    void set_configuration( const run_configuration& config );
    run_configuration get_configuration();

  private:
    void create_controls();

    wxSizer* create_program_sizer();
    wxSizer* create_arguments_sizer();

    void on_browse_program( wxCommandEvent& event );
    void on_add_argument( wxCommandEvent& event );
    void on_remove_argument( wxCommandEvent& event );
    void on_edit_argument( wxCommandEvent& event );

  private:
    /** \brief The text input in which the path to the executable is set. */
    wxTextCtrl* m_program_path;

    /** \brief The list of the arguments to pass to the program. */
    wxListBox* m_arguments;

    DECLARE_EVENT_TABLE()

  }; // class run_configuration_frame
} // namespace bf

#endif // __BF_RUN_CONFIGURATION_FRAME_HPP__
