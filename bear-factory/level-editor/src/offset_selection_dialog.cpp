/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::offset_selection_dialog class.
 * \author Julien Jorge
 */
#include "bf/offset_selection_dialog.hpp"

#include <wx/sizer.h>
#include <wx/stattext.h>

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 */
bf::offset_selection_dialog::offset_selection_dialog( wxWindow& parent )
  : wxDialog( &parent, wxID_ANY, wxString(_("Offset")) )
{
  create_controls();
  create_sizers();

  Fit();
} // offset_selection_dialog::offset_selection_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the offset on the x-axis.
 */
double bf::offset_selection_dialog::get_x() const
{
  return m_x->GetValue();
} // offset_selection_dialog::get_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the offset on the y-axis.
 */
double bf::offset_selection_dialog::get_y() const
{
  return m_y->GetValue();
} // offset_selection_dialog::get_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
void bf::offset_selection_dialog::create_controls()
{
  m_x = new spin_ctrl<double>(this);
  m_x->SetRange
    ( -std::numeric_limits<double>::max(), std::numeric_limits<double>::max() );
  m_x->SetValue(0);

  m_y = new spin_ctrl<double>(this);
  m_y->SetRange
    ( -std::numeric_limits<double>::max(), std::numeric_limits<double>::max() );
  m_y->SetValue(0);
} // offset_selection_dialog::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizers.
 */
void bf::offset_selection_dialog::create_sizers()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  wxStaticBoxSizer* s_sizer =
    new wxStaticBoxSizer( wxHORIZONTAL, this, _("Offset"));

  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Horizontal:")), 0 );
  s_sizer->Add( m_x, 0 );
  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Vertical:")), 0 );
  s_sizer->Add( m_y, 0 );

  sizer->Add( s_sizer, 1, wxEXPAND | wxALL, 5 );

  sizer->Add
    ( CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxCENTER, 5 );

  SetSizer(sizer);
} // offset_selection_dialog::create_sizers()
