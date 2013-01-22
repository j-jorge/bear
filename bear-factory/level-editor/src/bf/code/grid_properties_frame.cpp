/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::grid_properties_frame class.
 * \author Sebastien Angibaud
 */
#include "bf/grid_properties_frame.hpp"
#include <limits>
#include <algorithm> // for min, max

/*----------------------------------------------------------------------------*/
const unsigned int bf::grid_properties_frame::s_min_step_x(10);
const unsigned int bf::grid_properties_frame::s_min_step_y(10);

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this window.
 * \param g A grid in which we take the initial values.
 */
bf::grid_properties_frame::grid_properties_frame
( wxWindow* parent, const grid& g )
  : wxDialog(parent, wxID_ANY, wxString(_("Grid parameters"))), m_grid(g)
{
  create_controls();
  fill_controls();
  Fit();
} // grid_properties_frame::grid_properties_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the configured grid.
 */
const bf::grid& bf::grid_properties_frame::get_grid() const
{
  return m_grid;
} // grid_properties_frame::get_grid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the controls with the values of the grid.
 */
void bf::grid_properties_frame::fill_controls()
{
  m_step_x_text->SetRange( s_min_step_x, std::numeric_limits<int>::max() );
  m_step_y_text->SetRange( s_min_step_y, std::numeric_limits<int>::max() );
  m_step_x_text->SetValue( wxString::Format(wxT("%d"), m_grid.get_step().x) );
  m_step_y_text->SetValue( wxString::Format(wxT("%d"), m_grid.get_step().y) );

  m_offset_x_text->SetRange( 0, m_grid.get_step().x - 1 );
  m_offset_y_text->SetRange( 0, m_grid.get_step().y - 1 );
  m_offset_x_text->SetValue
    ( wxString::Format(wxT("%d"), m_grid.get_offset().x) );
  m_offset_y_text->SetValue
    ( wxString::Format(wxT("%d"), m_grid.get_offset().y) );

  m_magnetism_active_box->SetValue( m_grid.get_magnetism_active() );

  int min = std::min( m_grid.get_step().x, m_grid.get_step().y );

  m_magnetism_force_slider->SetRange( 1, min/10 +1 );
  m_magnetism_force_slider->SetValue( m_grid.get_magnetism_force() );
} // grid_properties_frame::fill_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::grid_properties_frame::create_controls()
{
  create_member_controls();
  create_sizer_controls();
} // grid_properties_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief for which we keep a reference.
 */
void bf::grid_properties_frame::create_member_controls()
{
  m_offset_x_text = new wxSpinCtrl( this, IDC_CHANGE_POSITION_SIZE );
  m_offset_y_text = new wxSpinCtrl( this, IDC_CHANGE_POSITION_SIZE );
  m_step_x_text = new wxSpinCtrl( this, IDC_CHANGE_POSITION_SIZE );
  m_step_y_text = new wxSpinCtrl( this, IDC_CHANGE_POSITION_SIZE );

  m_magnetism_active_box = new wxCheckBox( this, wxID_ANY, _("activated") );

  int min = std::min(s_min_step_x, s_min_step_y);

  m_magnetism_force_slider =
    new wxSlider( this, wxID_ANY, 1, 1, min/10 + 1, wxDefaultPosition,
      wxDefaultSize, wxSL_LABELS );
} // grid_properties_frame::create_member_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::grid_properties_frame::create_sizer_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  // offset
  wxStaticBoxSizer* s_sizer =
    new wxStaticBoxSizer( wxHORIZONTAL, this, _("Offset") );
  s_sizer->AddSpacer(5);
  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("X:")),
                0, wxALIGN_CENTRE_VERTICAL | wxALL, 0 );
  s_sizer->Add( m_offset_x_text, 1, wxALIGN_CENTRE_VERTICAL | wxALL, 0 );
  s_sizer->AddSpacer(20);
  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Y:")),
                0, wxALIGN_CENTRE_VERTICAL | wxALL, 0 );
  s_sizer->Add( m_offset_y_text, 1, wxALIGN_CENTRE_VERTICAL | wxALL, 0 );
  s_sizer->AddSpacer(5);

  sizer->AddSpacer(5);
  sizer->Add( s_sizer, 1, wxEXPAND | wxLEFT | wxRIGHT, 5 );
  sizer->AddSpacer(5);

  // step
  s_sizer = new wxStaticBoxSizer( wxHORIZONTAL, this, _("Step") );
  s_sizer->AddSpacer(5);
  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("X:")),
                0, wxALIGN_CENTRE_VERTICAL | wxALL, 0 );
  s_sizer->Add( m_step_x_text, 1, wxALIGN_CENTRE_VERTICAL | wxALL, 0 );
  s_sizer->AddSpacer(20);
  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Y:")),
                0, wxALIGN_CENTRE_VERTICAL | wxALL, 0 );
  s_sizer->Add( m_step_y_text, 1, wxALIGN_CENTRE_VERTICAL | wxALL, 0 );
  s_sizer->AddSpacer(5);

  sizer->Add( s_sizer, 1, wxEXPAND | wxLEFT | wxRIGHT, 5 );
  sizer->AddSpacer(5);

  s_sizer = new wxStaticBoxSizer( wxHORIZONTAL, this, _("Magnetism") );

  s_sizer->Add( m_magnetism_active_box, 0, wxEXPAND , 0 );
  s_sizer->Add( m_magnetism_force_slider, 1, wxEXPAND, 0 );

  sizer->Add( s_sizer, 1, wxEXPAND | wxLEFT | wxRIGHT, 5 );

  sizer->AddSpacer(5);
  sizer->Add( CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0,
              wxALL | wxCENTER , 0 );
  sizer->AddSpacer(2);
  SetSizer(sizer);
} // grid_properties_frame::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "OK" button.
 * \param event This event occured.
 */
void bf::grid_properties_frame::on_ok( wxCommandEvent& WXUNUSED(event) )
{
  m_grid.set_offset
    ( wxSize(m_offset_x_text->GetValue(), m_offset_y_text->GetValue()) );
  m_grid.set_step
    ( wxSize(m_step_x_text->GetValue(), m_step_y_text->GetValue()) );
  m_grid.set_magnetism_active( m_magnetism_active_box->IsChecked() );
  m_grid.set_magnetism_force( m_magnetism_force_slider->GetValue() );

  EndModal(wxID_OK);
} // grid_properties_frame::on_ok()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer a new value in spincontrol.
 * \param event This event occured.
 */
void bf::grid_properties_frame::on_value_change( wxSpinEvent& WXUNUSED(event) )
{
  m_offset_x_text->SetRange( 0, m_step_x_text->GetValue()-1 );
  m_offset_y_text->SetRange( 0, m_step_y_text->GetValue()-1 );

  int min = std::min(m_step_x_text->GetValue(), m_step_y_text->GetValue());
  m_magnetism_force_slider->SetRange( 1, min/10 + 1 );
} // grid_properties_frame::on_value_change()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::grid_properties_frame, wxDialog)
  EVT_BUTTON( wxID_OK, bf::grid_properties_frame::on_ok )
  EVT_SPINCTRL( bf::grid_properties_frame::IDC_CHANGE_POSITION_SIZE,
                bf::grid_properties_frame::on_value_change )
END_EVENT_TABLE()
