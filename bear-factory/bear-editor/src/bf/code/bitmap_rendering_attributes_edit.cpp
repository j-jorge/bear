/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::bitmap_rendering_attributes_edit class.
 * \author Sebastien Angibaud
 */
#include "bf/bitmap_rendering_attributes_edit.hpp"

#include "bf/wx_facilities.hpp"

#include <limits>

#include <wx/colordlg.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this window.
 * \param att The initial bitmap_rendering_attributes.
 */
bf::bitmap_rendering_attributes_edit::bitmap_rendering_attributes_edit
( wxWindow& parent, const bitmap_rendering_attributes& att )
  : wxPanel(&parent, wxID_ANY), base_edit<bitmap_rendering_attributes>(att)
{
  create_controls();
  Fit();
} // bitmap_rendering_attributes_edit::bitmap_rendering_attributes_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the displayed value is correct and, if it is, set the
 *        value according to the display.
 */
bool bf::bitmap_rendering_attributes_edit::validate()
{
  bitmap_rendering_attributes att;

  att.set_auto_size( m_auto_size->IsChecked() );

  att.set_width( m_width_spin->GetValue() );
  att.set_height( m_height_spin->GetValue() );

  if ( m_flip_x_box->Get3StateValue() == wxCHK_UNCHECKED )
    att.mirror( trinary_logic::boolean_false );
  else if ( m_flip_x_box->Get3StateValue() == wxCHK_CHECKED )
    att.mirror( trinary_logic::boolean_true );
  else
    att.mirror( trinary_logic::boolean_random );

  if ( m_flip_y_box->Get3StateValue() == wxCHK_UNCHECKED )
    att.flip( trinary_logic::boolean_false );
  else if ( m_flip_y_box->Get3StateValue() == wxCHK_CHECKED )
    att.flip( trinary_logic::boolean_true );
  else
    att.flip( trinary_logic::boolean_random );

  m_color_ctrl->validate();
  std::cout << m_color_ctrl->get_value().get_red_intensity()<< std::endl;
  att.set_color( m_color_ctrl->get_value() );
  att.set_angle( m_angle_spin->GetValue() );

  set_value(att);

  return true;
} // bitmap_rendering_attributes_edit::validate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after changing the value by set_value().
 */
void bf::bitmap_rendering_attributes_edit::value_updated()
{
  fill_controls();
} // bitmap_rendering_attributes_edit::value_updated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the controls with the values of the bitmap_rendering_attributes.
 */
void bf::bitmap_rendering_attributes_edit::fill_controls()
{
  bitmap_rendering_attributes att = get_value();

  m_auto_size->SetValue( att.get_auto_size() );

  m_width_spin->Enable( !att.get_auto_size() );
  m_height_spin->Enable( !att.get_auto_size() );

  m_width_spin->SetValue( att.width() );
  m_height_spin->SetValue( att.height() );

  if ( att.get_mirrored_status() == trinary_logic::boolean_false )
    m_flip_x_box->Set3StateValue(wxCHK_UNCHECKED);
  else if ( att.get_mirrored_status() == trinary_logic::boolean_true )
    m_flip_x_box->Set3StateValue(wxCHK_CHECKED);
  else
    m_flip_x_box->Set3StateValue(wxCHK_UNDETERMINED);

  if ( att.get_flipped_status() == trinary_logic::boolean_false )
    m_flip_y_box->Set3StateValue(wxCHK_UNCHECKED);
  else if ( att.get_flipped_status() == trinary_logic::boolean_true )
    m_flip_y_box->Set3StateValue(wxCHK_CHECKED);
  else
    m_flip_y_box->Set3StateValue(wxCHK_UNDETERMINED);

  m_color_ctrl->set_value( att.get_color() );
  m_angle_spin->SetValue( att.get_angle() );
} // bitmap_rendering_attributes_edit::fill_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::bitmap_rendering_attributes_edit::create_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  sizer->Add(create_size_and_placement_sizer(), 0, wxEXPAND, 0);
  sizer->Add(create_color_sizer(), 0, wxEXPAND, 0);
  SetSizer(sizer);

  fill_controls();
} // bitmap_rendering_attributes_edit::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and the sizer for the size and placement position
 *        part.
 */
wxSizer* bf::bitmap_rendering_attributes_edit::create_size_and_placement_sizer()
{
  wxStaticBoxSizer* result =
    new wxStaticBoxSizer( wxVERTICAL, this, _("Size and placement") );

  m_auto_size = new wxCheckBox( this, ID_AUTO_SIZE, _("Auto size") );
  m_width_spin = new wxSpinCtrl( this, wxID_ANY );
  m_height_spin = new wxSpinCtrl( this, wxID_ANY );
  m_flip_x_box = new wxCheckBox
    ( this, wxID_ANY, _("mirror"), wxDefaultPosition, wxDefaultSize,
      wxCHK_3STATE | wxCHK_ALLOW_3RD_STATE_FOR_USER);
  m_flip_y_box = new wxCheckBox
    ( this, wxID_ANY, _("flip"), wxDefaultPosition, wxDefaultSize,
      wxCHK_3STATE | wxCHK_ALLOW_3RD_STATE_FOR_USER);
  m_angle_spin =
    new spin_ctrl<double>
    ( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxEXPAND,
      -std::numeric_limits<double>::infinity(),
      std::numeric_limits<double>::infinity(), 0, 0.01 );

  m_width_spin->SetRange( 0, std::numeric_limits<int>::max() );
  m_height_spin->SetRange( 0, std::numeric_limits<int>::max() );

  wxBoxSizer* sub_sizer = new wxBoxSizer( wxHORIZONTAL );
  sub_sizer->Add( m_auto_size, 0, wxEXPAND );
  sub_sizer->Add( new wxStaticText(this, wxID_ANY, _("Width:")),
               1, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  sub_sizer->Add( m_width_spin, 0, wxALL, 5 );

  sub_sizer->Add( new wxStaticText(this, wxID_ANY, _("Height:")),
               1, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  sub_sizer->Add( m_height_spin, 0, wxALL, 5 );

  result->Add( sub_sizer, 0, wxEXPAND, 0 );
  sub_sizer = new wxBoxSizer( wxHORIZONTAL );

  sub_sizer->Add( m_flip_x_box, 0, wxEXPAND, 0 );
  sub_sizer->Add( m_flip_y_box, 0, wxEXPAND, 0 );
  sub_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Angle:")),
      1, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  sub_sizer->Add( m_angle_spin, 1, wxEXPAND, 0 );

  result->Add( sub_sizer, 0, wxEXPAND, 0 );

  return result;
} // bitmap_rendering_attributes_edit::create_size_and_placement_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and the sizer for the color part.
 */
wxSizer* bf::bitmap_rendering_attributes_edit::create_color_sizer()
{
  m_color_ctrl = new color_edit( this );

  wxStaticBoxSizer* result =
    new wxStaticBoxSizer( wxHORIZONTAL, this, _("Color") );

  result->Add( m_color_ctrl, 0, wxEXPAND, 0 );
  
  return result;
} // bitmap_rendering_attributes_edit::create_color_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent when the auto size checkbox changes.
 * \param event The event.
 */
void bf::bitmap_rendering_attributes_edit::on_auto_size
( wxCommandEvent& WXUNUSED(event) )
{
  m_width_spin->Enable( !m_auto_size->IsChecked() );
  m_height_spin->Enable( !m_auto_size->IsChecked() );
} // bitmap_rendering_attributes_edit::on_auto_size()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::bitmap_rendering_attributes_edit, wxPanel)
  EVT_CHECKBOX( bf::bitmap_rendering_attributes_edit::ID_AUTO_SIZE,
                bf::bitmap_rendering_attributes_edit::on_auto_size )
END_EVENT_TABLE()
