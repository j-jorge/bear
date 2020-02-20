/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::color_edit class.
 * \author Sébastien Angibaud
 */
#include "bf/color_edit.hpp"

#include "bf/path_configuration.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this one.
 * \param v The initial value.
 */
bf::color_edit::color_edit
( wxWindow& parent, const color& v )
  : wxPanel( &parent, wxID_ANY ), base_edit<color>(v)
{
  create_controls();
} // color_edit::color_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this one.
 * \param v The initial value.
 */
bf::color_edit::color_edit
( wxWindow* parent, const color& v )
  : wxPanel( parent, wxID_ANY ), base_edit<color>(v)
{
  create_controls();
} // color_edit::color_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Checks if the displayed value is correct and, if it is, set the
 *        value according to the display.
 */
bool bf::color_edit::validate()
{
  set_value( make_color() );
  
  return true;
} // color_edit::validate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the color corresponding to the values in the control.
 */
bf::color bf::color_edit::make_color() const
{
  color result;

  wxColor c = m_picker->GetColour();
  result.set_opacity( m_opacity_spin->GetValue() );
  result.set_intensity
    ( (double)c.Red() / 255.0, (double)c.Green() / 255.0, 
      (double)c.Blue() / 255.0);

  return result;
} // color_edit::make_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after changing the value by set_value().
 */
void bf::color_edit::value_updated()
{
  fill_controls();
} // color_edit::value_updated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fills the controls with the values of the color.
 */
void bf::color_edit::fill_controls()
{
  color f = get_value();

  m_picker->SetColour
    ( wxColour
      ( (int)(unsigned char)(255.0 * f.get_red_intensity()), 
        (int)(unsigned char)(255.0 * f.get_green_intensity()), 
        (int)(unsigned char)(255.0 * f.get_blue_intensity()) ) );

  m_opacity_spin->SetValue(f.get_opacity());
} // color_edit::fill_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the controls of the window.
 */
void bf::color_edit::create_controls()
{
  m_picker = new wxColourPickerCtrl( this, wxID_ANY );

  m_opacity_spin = 
    new spin_ctrl<double>
    ( this, wxID_ANY, wxDefaultPosition,
      wxDefaultSize, wxEXPAND, 0, 1, 1, 0.01 );
  m_opacity_spin->SetRange( 0, 1 );
  
  create_sizer_controls();
  fill_controls();
} // color_edit::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the controls and adds them in sizers.
 */
void bf::color_edit::create_sizer_controls()
{
  wxBoxSizer* v_sizer = new wxBoxSizer( wxVERTICAL );

  v_sizer->Add( m_picker, 0, wxEXPAND, 0 );

  wxBoxSizer* h_sizer = new wxBoxSizer( wxHORIZONTAL );

  h_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Opacity:")),
      1, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  h_sizer->Add( m_opacity_spin, 0, wxALL, 5 );

  v_sizer->Add( h_sizer, 0, wxEXPAND );

  SetSizer(v_sizer);
} // color_edit::create_sizer_controls()

