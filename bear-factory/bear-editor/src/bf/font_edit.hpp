/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Control for editing a field whose value is a font.
 * \author Sébastien Angibaud
 */
#ifndef __BF_FONT_EDIT_HPP__
#define __BF_FONT_EDIT_HPP__

#include "bf/base_edit.hpp"
#include "bf/default_value.hpp"
#include "bf/font.hpp"

#include "bf/spin_ctrl.hpp"

#include "bf/libeditor_export.hpp"

#include <wx/wx.h>

namespace bf
{
  class workspace_environment;

  /**
   * \brief Control for editing a field whose value is a a font.
   * \author Sébastien Angibaud
   */
  class BEAR_EDITOR_EXPORT font_edit:
    public wxPanel,
    public base_edit<font>
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_FONT_SELECT
      }; // enum control_id

  public:
    font_edit
      ( wxWindow& parent, workspace_environment* env, 
        const font& v = default_value<font>::get() );

    bool validate();

  private:
    font make_font() const;

    void value_updated();
    void fill_controls();

    void create_controls();
    void create_sizer_controls();
    wxSizer* create_size_sizer();
    wxSizer* create_font_name_sizer();

    void on_font_select(wxCommandEvent& event);

  private:
    /** \brief The control for setting the size of the font. */
    bf::spin_ctrl<double>* m_size_spin;

    /** \brief The control in which we display the file name. */
    wxTextCtrl* m_font_name_text;

    /** \brief The workspace name. */
    workspace_environment* m_workspace;

    DECLARE_EVENT_TABLE()

  }; // class font_edit
} // namespace bf

#endif // __BF_FONT_EDIT_HPP__
