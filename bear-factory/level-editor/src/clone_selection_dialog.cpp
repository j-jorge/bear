/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::clone_selection_dialog class.
 * \author Julien Jorge
 */
#include "bf/clone_selection_dialog.hpp"

#include <wx/sizer.h>
#include <wx/stattext.h>

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 */
bf::clone_selection_dialog::clone_selection_dialog( wxWindow& parent )
  : wxDialog( &parent, wxID_ANY, wxString(_("Clone selection")) )
{
  create_controls();
  create_sizers();

  Fit();
} // clone_selection_dialog::clone_selection_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the count of clones on the x-axis.
 */
unsigned int bf::clone_selection_dialog::get_x_count() const
{
  return m_x_count->GetValue();
} // clone_selection_dialog::get_x_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the count of clones on the y-axis.
 */
unsigned int bf::clone_selection_dialog::get_y_count() const
{
  return m_y_count->GetValue();
} // clone_selection_dialog::get_y_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the offset on the x-axis.
 */
double bf::clone_selection_dialog::get_x_offset() const
{
  return m_x_offset->GetValue();
} // clone_selection_dialog::get_x_offset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the offset on the y-axis.
 */
double bf::clone_selection_dialog::get_y_offset() const
{
  return m_y_offset->GetValue();
} // clone_selection_dialog::get_y_offset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the clones are added to the selection.
 */
bool bf::clone_selection_dialog::get_add_to_selection() const
{
  return m_add_to_selection->GetValue();
} // clone_selection_dialog::get_add_to_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
void bf::clone_selection_dialog::create_controls()
{
  m_x_count = new wxSpinCtrl(this);
  m_x_count->SetRange(1, std::numeric_limits<int>::max());
  m_x_count->SetValue(1);

  m_y_count = new wxSpinCtrl(this);
  m_y_count->SetRange(1, std::numeric_limits<int>::max());
  m_y_count->SetValue(1);

  m_x_offset = new spin_ctrl<double>(this);
  m_x_offset->SetRange
    ( -std::numeric_limits<double>::max(), std::numeric_limits<double>::max() );
  m_x_offset->SetValue(0);

  m_y_offset = new spin_ctrl<double>(this);
  m_y_offset->SetRange
    ( -std::numeric_limits<double>::max(), std::numeric_limits<double>::max() );
  m_y_offset->SetValue(0);

  m_add_to_selection = new wxCheckBox(this, wxID_ANY, _("Add to selection"));
  m_add_to_selection->SetValue(true);
} // clone_selection_dialog::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
void bf::clone_selection_dialog::create_sizers()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  wxStaticBoxSizer* s_sizer =
    new wxStaticBoxSizer( wxHORIZONTAL, this, _("Horizontal"));

  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Count:")), 0 );
  s_sizer->Add( m_x_count, 0 );

  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Offset:")), 0 );
  s_sizer->Add( m_x_offset, 0 );

  sizer->Add( s_sizer, 1, wxEXPAND | wxALL, 5 );

  s_sizer = new wxStaticBoxSizer( wxHORIZONTAL, this, _("Vertical"));

  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Count:")), 0 );
  s_sizer->Add( m_y_count, 0 );

  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Offset:")), 0 );
  s_sizer->Add( m_y_offset, 0 );

  sizer->Add( s_sizer, 1, wxEXPAND | wxALL, 5 );
  sizer->Add( m_add_to_selection, 1, wxEXPAND | wxALL, 5 );
  sizer->Add
    ( CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxCENTER, 5 );

  SetSizer(sizer);
} // clone_selection_dialog::create_sizers()
