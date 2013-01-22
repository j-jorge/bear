/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::about_dialog class.
 * \author Julien Jorge
 */
#include "bf/about_dialog.hpp"

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "bf/version.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 */
bf::about_dialog::about_dialog( wxWindow& parent )
  : wxDialog( &parent, wxID_ANY, _("About"), wxDefaultPosition,
              wxSize(300, 200), wxDEFAULT_DIALOG_STYLE )
{
  create_controls();
} // about_dialog::about_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
void bf::about_dialog::create_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );
  wxStaticText* text =
    new wxStaticText( this, wxID_ANY, _("Bear Factory"), wxDefaultPosition,
                      wxDefaultSize, wxALIGN_CENTRE );
  wxFont fnt;
  fnt.SetPointSize(20);
  fnt.SetWeight(wxFONTWEIGHT_BOLD);
  text->SetFont(fnt);

  sizer->Add( text, 0, wxEXPAND | wxALL, 5 );

  text = new wxStaticText
    ( this, wxID_ANY,
      wxString::Format(_("Release %d.%d.%d, Animation editor"),
                       BF_MAJOR_VERSION, BF_MINOR_VERSION, BF_RELEASE_NUMBER),
      wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );

  sizer->Add( text, 0, wxEXPAND | wxALL, 5 );

  const wxString contrib_text =
    wxT("Copyright\n")
    wxT("\tS\u00E9bastien Angibaud\n")
    wxT("\tJulien Jorge\n");

  wxTextCtrl* contrib =
    new wxTextCtrl( this, wxID_ANY, contrib_text, wxDefaultPosition,
                    wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY );

  sizer->Add( contrib, 1, wxEXPAND | wxALL, 5 );
  sizer->Add( CreateStdDialogButtonSizer(wxOK), 0, wxALL | wxCENTER, 5 );

  SetSizer(sizer);
} // about_dialog::create_controls()
