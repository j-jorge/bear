/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::run_configuration_frame class.
 * \author Julien Jorge
 */
#include "bf/run_configuration_frame.hpp"

#include "bf/wx_facilities.hpp"

#include <wx/button.h>
#include <wx/filedlg.h>
#include <wx/sizer.h>
#include <wx/textdlg.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent frame of this frame.
 */
bf::run_configuration_frame::run_configuration_frame( wxWindow* parent )
  : wxDialog(parent, wxID_ANY, _("Run configuration"))
{
  create_controls();
} // run_configuration_frame::run_configuration_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the dialog ith a given configuration.
 * \param config The configuration.
 */
void bf::run_configuration_frame::set_configuration
( const run_configuration& config )
{
  m_program_path->SetValue( std_to_wx_string(config.get_program_path()) );

  m_arguments->Clear();

  for ( std::size_t i=0; i!=config.get_arguments_count(); ++i )
    m_arguments->Append( std_to_wx_string(config.get_argument(i)) );
} // run_configuration_frame::set_configuration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a run configuration corresponding to the data in the dialog.
 */
bf::run_configuration bf::run_configuration_frame::get_configuration()
{
  run_configuration result;
  result.set_program_path( wx_to_std_string(m_program_path->GetValue()) );

  for ( std::size_t i=0; i!=m_arguments->GetCount(); ++i )
    result.append_argument( wx_to_std_string(m_arguments->GetString(i)) );

  return result;
} // run_configuration_frame::get_configuration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of this dialog.
 */
void bf::run_configuration_frame::create_controls()
{
  wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  sizer->Add( create_program_sizer(), 0, wxEXPAND );
  sizer->Add( create_arguments_sizer(), 1, wxEXPAND );
  //sizer->Add( create_items_sizer(), 1, wxEXPAND );

  sizer->Add( CreateButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND );

  SetSizer( sizer );
} // run_configuration_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizer containing the controls to select the program to run.
 */
wxSizer* bf::run_configuration_frame::create_program_sizer()
{
  wxSizer* result = new wxStaticBoxSizer( wxHORIZONTAL, this, _("Program") );

  m_program_path = new wxTextCtrl( this, wxID_ANY );
  result->Add( m_program_path, 1, wxEXPAND );

  wxButton* btn = new wxButton( this, ID_BROWSE, wxT("...") );
  result->Add( btn, 0, wxEXPAND );

  return result;
} // run_configuration_frame::create_program_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizer containing the list of the arguments.
 */
wxSizer* bf::run_configuration_frame::create_arguments_sizer()
{
  wxSizer* result = new wxStaticBoxSizer( wxHORIZONTAL, this, _("Arguments") );
  m_arguments = new wxListBox( this, ID_ARGUMENTS );

  wxSizer* button_sizer = new wxBoxSizer( wxVERTICAL );

  wxButton* btn = new wxButton( this, ID_ADD_ARGUMENT, wxT("+") );
  button_sizer->Add( btn, 0, wxEXPAND );

  btn = new wxButton( this, ID_REMOVE_ARGUMENT, wxT("-") );
  button_sizer->Add( btn, 0, wxEXPAND );

  result->Add( m_arguments, 1, wxEXPAND );
  result->Add( button_sizer, 0, wxEXPAND );

  return result;
} // run_configuration_frame::create_arguments_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the program to run.
 * \param event Not used.
 */
void bf::run_configuration_frame::on_browse_program
( wxCommandEvent& WXUNUSED(event) )
{
  wxFileDialog dlg(this);
  dlg.SetPath( m_program_path->GetValue() );

  if ( dlg.ShowModal() == wxID_OK )
    m_program_path->SetValue( dlg.GetPath() );
} // run_configuration_frame::on_browse_program()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the program to run.
 * \param event Not used.
 */
void bf::run_configuration_frame::on_add_argument
( wxCommandEvent& WXUNUSED(event) )
{
  wxTextEntryDialog dlg(this, _("Enter an argument for the program"));

  if ( dlg.ShowModal() == wxID_OK )
    m_arguments->Append( dlg.GetValue() );
} // run_configuration_frame::on_add_argument()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the program to run.
 * \param event Not used.
 */
void bf::run_configuration_frame::on_remove_argument
( wxCommandEvent& WXUNUSED(event) )
{
  const int index = m_arguments->GetSelection();

  if ( index != wxNOT_FOUND )
    {
      m_arguments->Delete( index );
      m_arguments->SetSelection
        ( std::min( index, (int)m_arguments->GetCount() - 1 ) );
    }
} // run_configuration_frame::on_remove_argument()

/*----------------------------------------------------------------------------*/
/**
 * \brief Edit the selected argument.
 * \param event The event carrying the index of the argument to edit.
 */
void bf::run_configuration_frame::on_edit_argument( wxCommandEvent& event )
{
  int index = event.GetInt();

  wxTextEntryDialog dlg( this, _("Enter an argument for the program") );
  dlg.SetValue( m_arguments->GetString(index) );

  if ( dlg.ShowModal() == wxID_OK )
    m_arguments->SetString( index, dlg.GetValue() );
} // run_configuration_frame::on_edit_argument()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::run_configuration_frame, wxDialog)
  EVT_BUTTON( bf::run_configuration_frame::ID_BROWSE,
              bf::run_configuration_frame::on_browse_program )
  EVT_BUTTON( bf::run_configuration_frame::ID_ADD_ARGUMENT,
              bf::run_configuration_frame::on_add_argument )
  EVT_BUTTON( bf::run_configuration_frame::ID_REMOVE_ARGUMENT,
              bf::run_configuration_frame::on_remove_argument )
  EVT_LISTBOX_DCLICK( bf::run_configuration_frame::ID_ARGUMENTS,
                      bf::run_configuration_frame::on_edit_argument )
END_EVENT_TABLE()
