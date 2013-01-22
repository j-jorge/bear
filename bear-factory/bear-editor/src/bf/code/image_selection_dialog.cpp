/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::image_selection_dialog class.
 * \author Julien Jorge
 */
#include "bf/image_selection_dialog.hpp"

#include "bf/image_list_ctrl.hpp"
#include "bf/image_pool.hpp"

#include <wx/sizer.h>
#include <wx/stattext.h>

/*----------------------------------------------------------------------------*/
wxString bf::image_selection_dialog::s_previous_pattern;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param val The name of the current selected image.
 */
bf::image_selection_dialog::image_selection_dialog
( wxWindow& parent, const wxString& val )
  : wxDialog( &parent, wxID_ANY, _("Choose an image"), wxDefaultPosition,
              wxSize(640, 480), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER )
{
  create_controls();
  fill_image_list();

  m_image_list->set_selection(val);
} // image_selection_dialog::image_selection_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the selected image.
 */
wxString bf::image_selection_dialog::get_image_name() const
{
  return m_image_list->get_selection();
} // image_selection_dialog::get_image_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
void bf::image_selection_dialog::create_controls()
{
  m_image_list = new image_list_ctrl( *this );
  m_pattern = new wxTextCtrl( this, IDC_PATTERN_TEXT );
  m_pattern->SetValue(s_previous_pattern);

  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  sizer->Add( m_image_list, 1, wxEXPAND | wxALL, 5 );

  wxBoxSizer* s_sizer = new wxBoxSizer( wxHORIZONTAL );
  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Pattern:")), 0 );
  s_sizer->Add( m_pattern, 1, wxEXPAND );

  sizer->Add( s_sizer, 0, wxEXPAND | wxALL, 5 );
  sizer->Add
    ( CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxCENTER, 5 );

  SetSizer(sizer);
} // image_selection_dialog::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the list of images with the images that match the pattern.
 */
void bf::image_selection_dialog::fill_image_list()
{
  std::list<wxString> keep;
  wxString pat( m_pattern->GetValue() );

  image_pool::const_iterator it = image_pool::get_instance().begin();
  image_pool::const_iterator eit = image_pool::get_instance().end();

  if ( pat.IsEmpty() )
    pat = wxT("*");

  if ( (pat[0] != wxT('*')) && (pat[0] != wxT('?')) )
    pat = wxT("*") + pat;

  if ( (pat[pat.length() - 1] != wxT('*'))
       && (pat[pat.length() - 1] != wxT('?')) )
    pat += wxT("*");

  for (; it!=eit; ++it)
    if ( it->Matches( pat ) )
      keep.push_back( *it );

  m_image_list->set_list(keep);
} // image_selection_dialog::fill_image_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief The text of the pattern has changed.
 * \param event The event.
 */
void bf::image_selection_dialog::on_pattern_change
( wxCommandEvent& WXUNUSED(event) )
{
  s_previous_pattern = m_pattern->GetValue();
  fill_image_list();
} // image_selection_dialog::on_pattern_change()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::image_selection_dialog, wxDialog)
  EVT_TEXT( bf::image_selection_dialog::IDC_PATTERN_TEXT,
            bf::image_selection_dialog::on_pattern_change )
END_EVENT_TABLE()

