/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::font_edit class.
 * \author Sébastien Angibaud
 */
#include "bf/font_edit.hpp"

#include "bf/path_configuration.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this one.
 * \param v The initial value.
 */
bf::font_edit::font_edit
( wxWindow& parent, const font& v )
  : wxPanel( &parent, wxID_ANY ), base_edit<font>(v)
{
  create_controls();
} // font_edit::font_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Checks if the displayed value is correct and, if it is, set the
 *        value according to the display.
 */
bool bf::font_edit::validate()
{
  set_value( make_font() );
  return true;
} // font_edit::validate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the font corresponding to the values in the control.
 */
bf::font bf::font_edit::make_font() const
{
  font result;

  result.set_size( m_size_spin->GetValue() );
  result.set_font_name( wx_to_std_string( m_font_name_text->GetValue() ) );

  return result;
} // font_edit::make_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after changing the value by set_value().
 */
void bf::font_edit::value_updated()
{
  fill_controls();
} // font_edit::value_updated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fills the controls with the values of the font.
 */
void bf::font_edit::fill_controls()
{
  font f = get_value();

  m_size_spin->SetValue( f.get_size() );
  m_font_name_text->SetValue( std_to_wx_string( f.get_font_name() ) );
} // font_edit::fill_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the controls of the window.
 */
void bf::font_edit::create_controls()
{
  m_size_spin = new spin_ctrl<double>( this );
  m_size_spin->SetRange( 0, std::numeric_limits<double>::max() );

  m_font_name_text = new wxTextCtrl( this, wxID_ANY );

  create_sizer_controls();
  fill_controls();
} // font_edit::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the controls and adds them in sizers.
 */
void bf::font_edit::create_sizer_controls()
{
  wxBoxSizer* v_sizer = new wxBoxSizer( wxVERTICAL );

  v_sizer->Add( create_font_name_sizer(), 0, wxEXPAND );
  v_sizer->Add( create_size_sizer(), 0, wxEXPAND );

  SetSizer(v_sizer);
} // font_edit::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the controls and the sizer for the size part.
 */
wxSizer* bf::font_edit::create_size_sizer()
{
  wxBoxSizer* result = new wxBoxSizer( wxHORIZONTAL );

  result->Add
    ( new wxStaticText(this, wxID_ANY, _("Size:")),
      1, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  result->Add( m_size_spin, 0, wxALL, 5 );

  return result;
} // font_edit::create_size_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the controls and the sizer for the name of the font.
 */
wxSizer* bf::font_edit::create_font_name_sizer()
{
  wxBoxSizer* result = new wxBoxSizer( wxHORIZONTAL );

  wxButton* font_name_browse = new wxButton
    ( this, IDC_FONT_SELECT, wxT("..."), wxDefaultPosition, wxSize(30, -1) );

  result->Add
    ( new wxStaticText(this, wxID_ANY, _("Font:")),
      0, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  result->Add( m_font_name_text, 1, wxEXPAND | wxALL, 5 );
  result->Add( font_name_browse, 0, wxALL, 5 );

  return result;
} // font_edit::create_font_name_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the button to select a font.
 * \param event This event occured.
 */
void bf::font_edit::on_font_select( wxCommandEvent& WXUNUSED(event) )
{
  std::string p( wx_to_std_string( m_font_name_text->GetValue() ) );
  path_configuration::get_instance().get_full_path(p);

  wxFileDialog dlg
    ( this, _("Choose a font file"), wxEmptyString, std_to_wx_string(p),
      _("Font files|*.fnt;*.ttf"), wxFD_DEFAULT_STYLE | wxFD_FILE_MUST_EXIST );

  if (dlg.ShowModal() == wxID_OK)
    {
      std::string new_p( wx_to_std_string( dlg.GetPath() ) );
      path_configuration::get_instance().get_relative_path( new_p );

      m_font_name_text->SetValue( std_to_wx_string( new_p ) );
    }
} // font_edit::on_font_select()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::font_edit, wxPanel)
  EVT_BUTTON( bf::font_edit::IDC_FONT_SELECT,
              bf::font_edit::on_font_select )
END_EVENT_TABLE()
