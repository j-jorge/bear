/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::frame_duration_dialog class.
 * \author Julien Jorge
 */
#include "bf/frame_duration_dialog.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent Parent window.
 */
bf::frame_duration_dialog::frame_duration_dialog( wxWindow* parent )
  : wxDialog(parent, wxID_ANY, wxT("Change frame durations"), wxDefaultPosition,
             wxDefaultSize)
{
  create_controls();
  Fit();
} // frame_duration_dialog::frame_duration_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of the operand applied to the frames.
 */
double bf::frame_duration_dialog::get_operand() const
{
  return m_operand->GetValue();
} // frame_duration_dialog::get_operand()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell how to apply the operand to the durations.
 */
bf::frame_duration_dialog::duration_application_mode
bf::frame_duration_dialog::get_mode() const
{
  duration_application_mode result(duration_replace);

  switch ( m_mode->GetSelection() )
    {
    case 0: result = duration_replace; break;
    case 1: result = duration_add; break;
    case 2: result = duration_multiply; break;
    }

  return result;
} // frame_duration_dialog::get_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the dialog.
 */
void bf::frame_duration_dialog::create_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  wxArrayString choices;
  choices.Add(_("Replace previous duration."));
  choices.Add(_("Add to previous duration."));
  choices.Add(_("Multiply previous duration."));

  m_mode = new wxRadioBox
    (this, wxID_ANY, _("Application mode"), wxDefaultPosition, wxDefaultSize,
     choices);
  m_mode->SetSelection(0);

  m_operand = new spin_ctrl<double>(this);
  m_operand->SetRange(0, std::numeric_limits<double>::max());
  m_operand->SetValue(1);
  m_operand->SetStep(0.1);

  sizer->Add( m_mode, 1, wxEXPAND | wxALL, 5 );
  sizer->Add( m_operand, 0, wxEXPAND | wxALL, 5 );

  sizer->Add
    ( CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxCENTER, 5 );

  SetSizer(sizer);
} // frame_duration_dialog::create_controls()
