/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::sample_edit class.
 * \author Julien Jorge
 */
#include "bf/sample_edit.hpp"

#include "bf/path_configuration.hpp"
#include "bf/workspace_environment.hpp"
#include "bf/wx_facilities.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this window.
 * \param w The workspace name.
 * \param s The initial sample.
 */
bf::sample_edit::sample_edit
( wxWindow& parent, workspace_environment* env, const sample& s )
  : wxPanel(&parent, wxID_ANY), base_edit<sample>(s), m_workspace(env)
{
  create_controls();
  Fit();
} // sample_edit::sample_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the displayed value is correct and, if it is, set the
 *        value according to the display.
 */
bool bf::sample_edit::validate()
{
  sample s;

  s.set_loops( m_loops->GetValue() );
  s.set_volume( m_volume->GetValue() );
  s.set_path( wx_to_std_string(m_sound_file->GetValue()) );

  set_value(s);

  return true;
} // sample_edit::validate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after changing the value by set_value().
 */
void bf::sample_edit::value_updated()
{
  fill_controls();
} // sample_edit::value_updated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the controls with the values of the sample.
 */
void bf::sample_edit::fill_controls()
{
  sample s = get_value();

  m_loops->SetValue(s.get_loops());
  m_volume->SetValue(s.get_volume());
  m_sound_file->SetValue( std_to_wx_string(s.get_path()) );
} // sample_edit::fill_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::sample_edit::create_controls()
{
  m_loops = new wxSpinCtrl( this, wxID_ANY );
  m_volume = new spin_ctrl<double>( this, wxID_ANY );
  m_sound_file = new wxTextCtrl( this, wxID_ANY );

  m_loops->SetRange( 0, std::numeric_limits<int>::max() );
  m_volume->SetRange( 0, 1 );
  m_volume->SetStep(0.1);

  create_sizer_controls();

  fill_controls();
} // sample_edit::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::sample_edit::create_sizer_controls()
{
  wxBoxSizer* v_sizer = new wxBoxSizer( wxVERTICAL );
  wxBoxSizer* h_sizer = new wxBoxSizer( wxHORIZONTAL );

  h_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Sound file:")),
      0, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  h_sizer->Add( m_sound_file, 1, wxEXPAND | wxALL, 5 );

  wxButton* browse = new wxButton
    ( this, IDC_FILE_SELECT, wxT("..."), wxDefaultPosition, wxSize(30, -1) );

  h_sizer->Add( browse, 0, wxALL, 5 );

  v_sizer->Add( h_sizer, 0, wxEXPAND );

  h_sizer = new wxBoxSizer( wxHORIZONTAL );
  h_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Loops:")),
      1, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  h_sizer->Add( m_loops, 1, wxEXPAND );
  h_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Volume:")),
      1, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  h_sizer->Add( m_volume, 1, wxEXPAND );

  v_sizer->Add( h_sizer, 0, wxALL, 5 );

  SetSizer(v_sizer);
} // sample_edit::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the button to select a sound file.
 * \param event This event occured.
 */
void bf::sample_edit::on_file_select( wxCommandEvent& WXUNUSED(event) )
{
  std::string p = wx_to_std_string(m_sound_file->GetValue());
  path_configuration::get_instance().get_full_path
    (p, m_workspace->name);

  wxFileDialog dlg
    ( this, _("Choose a sound file"), wxEmptyString, std_to_wx_string(p),
      _("Sound files|*.ogg;*.wav|All files|*"),
      wxFD_DEFAULT_STYLE | wxFD_FILE_MUST_EXIST );

  if (dlg.ShowModal() == wxID_OK)
    {
      std::string new_p = wx_to_std_string( dlg.GetPath() );
      path_configuration::get_instance().get_relative_path
        (new_p, m_workspace->name);

      m_sound_file->SetValue( std_to_wx_string(new_p) );
    }
} // sample_edit::on_file_select()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::sample_edit, wxPanel)
  EVT_BUTTON( bf::sample_edit::IDC_FILE_SELECT,
              bf::sample_edit::on_file_select )
END_EVENT_TABLE()
