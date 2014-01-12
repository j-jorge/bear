/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The window showing the properties of a sample.
 * \author Julien Jorge
 */
#ifndef __BF_SAMPLE_EDIT_HPP__
#define __BF_SAMPLE_EDIT_HPP__

#include "bf/default_value.hpp"
#include "bf/libeditor_export.hpp"

#include "bf/base_edit.hpp"
#include "bf/sample.hpp"
#include "bf/spin_ctrl.hpp"

#include <wx/wx.h>
#include <wx/spinctrl.h>

#include <vector>
#include <claw/rectangle.hpp>

namespace bf
{
  class workspace_environment;

  /**
   * \brief The window showing the properties of a sample.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT sample_edit:
    public wxPanel,
    public base_edit<sample>
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_FILE_SELECT
      }; // enum control_id

  public:
    sample_edit
      ( wxWindow& parent, workspace_environment& env,
        const sample& s = default_value<sample>::get() );

    bool validate();

  private:
    void value_updated();
    void fill_controls();

    void create_controls();
    void create_sizer_controls();

    void on_file_select(wxCommandEvent& event);

  private:
    /** \brief How many times the sample is repeated. */
    wxSpinCtrl* m_loops;

    /** \brief The valume at which the sample is played. */
    spin_ctrl<double>* m_volume;

    /** \brief The path to the sample file. */
    wxTextCtrl* m_sound_file;

    /** \brief The workspace environment used. */
    workspace_environment& m_workspace;

    DECLARE_EVENT_TABLE()

  }; // class sample_edit
} // namespace bf

#endif // __BF_SAMPLE_EDIT_HPP__
