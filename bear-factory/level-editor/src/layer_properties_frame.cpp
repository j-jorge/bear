/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::layer_properties_frame class.
 * \author Julien Jorge
 */
#include "bf/layer_properties_frame.hpp"

#include "bf/layer.hpp"
#include "bf/wx_facilities.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
const unsigned int bf::layer_properties_frame::s_min_width(800);
const unsigned int bf::layer_properties_frame::s_min_height(600);

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this window.
 */
bf::layer_properties_frame::layer_properties_frame( wxWindow* parent )
  : wxDialog(parent, wxID_ANY, wxString(_("Layer properties")))
{
  create_controls();
  Fit();
} // layer_properties_frame::layer_properties_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the layer fits the level.
 */
bool bf::layer_properties_frame::get_layer_fits_level() const
{
  return m_fit_level->IsChecked();
} // layer_properties_frame::get_layer_fits_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of the layer.
 */
unsigned int bf::layer_properties_frame::get_layer_width() const
{
  return m_width->GetValue();
} // layer_properties_frame::get_layer_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of the layer.
 */
unsigned int bf::layer_properties_frame::get_layer_height() const
{
  return m_height->GetValue();
} // layer_properties_frame::get_layer_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the class of the layer.
 */
std::string bf::layer_properties_frame::get_layer_class_name() const
{
  return wx_to_std_string( m_class_name->GetStringSelection() );
} // layer_properties_frame::get_layer_class_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the layer.
 */
std::string bf::layer_properties_frame::get_layer_name() const
{
  return wx_to_std_string( m_name->GetValue() );
} // layer_properties_frame::get_layer_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the tag of the layer.
 */
std::string bf::layer_properties_frame::get_tag() const
{
  return wx_to_std_string( m_tag->GetValue() );
} // layer_properties_frame::get_tag()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the properties of the layer.
 * \param lay The layer from which we take the info.
 *
 * The new width is max(lay.get_width(), s_min_width), and the new height
 * max(lay.get_height(), s_min_height).
 */
void bf::layer_properties_frame::fill_from( const layer& lay )
{
  m_fit_level->SetValue( lay.fits_level() );
  m_width->SetValue
    ( wxString::Format(wxT("%d"), std::max(lay.get_width(), s_min_width)) );
  m_height->SetValue
    ( wxString::Format(wxT("%d"), std::max(lay.get_height(), s_min_height)) );

  m_height->Enable( !lay.fits_level() );
  m_width->Enable( !lay.fits_level() );
  m_name->SetValue( std_to_wx_string(lay.get_name()) );
  m_tag->SetValue( std_to_wx_string(lay.get_tag()) );

  unsigned int i=0;
  bool found = false;
  const wxString ref( std_to_wx_string(lay.get_class_name()) );

  while ( !found && (i!=m_class_name->GetCount()) )
    if ( m_class_name->GetString(i) == ref )
      found = true;
    else
      ++i;

  if ( found )
    m_class_name->SetSelection(i);
  else
    m_class_name->SetSelection(0);
} // layer_properties_frame::set_layer_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::layer_properties_frame::create_controls()
{
  create_member_controls();
  create_sizer_controls();
} // layer_properties_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief for which we keep a reference.
 */
void bf::layer_properties_frame::create_member_controls()
{
  wxArrayString layer_names;

  layer_names.Add( wxT("action_layer") );
  layer_names.Add( wxT("decoration_layer") );
  layer_names.Add( wxT("pattern_layer") );

  m_fit_level =
    new wxCheckBox( this, IDC_FIT_LEVEL_SIZE, _("Match level size.") );
  m_width = new wxSpinCtrl( this, wxID_ANY );
  m_height = new wxSpinCtrl( this, wxID_ANY );
  m_class_name = new wxChoice
    ( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, layer_names );
  m_name = new wxTextCtrl( this, wxID_ANY, wxT("") );
  m_tag = new wxTextCtrl( this, wxID_ANY, wxT("") );

  m_width->SetRange( s_min_width, std::numeric_limits<int>::max() );
  m_height->SetRange( s_min_height, std::numeric_limits<int>::max() );
} // layer_properties_frame::create_member_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::layer_properties_frame::create_sizer_controls()
{
  wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

  sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Width")), 1, wxEXPAND | wxALL, 5 );
  sizer->Add( m_width, 0  );
  sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Height")), 1, wxEXPAND | wxALL, 5 );
  sizer->Add( m_height, 0 );

  // Size
  wxSizer* s_sizer = new wxStaticBoxSizer( wxVERTICAL, this, _("Size") );
  s_sizer->Add( m_fit_level, 0 );
  s_sizer->Add( sizer, 0 );
  sizer = new wxBoxSizer( wxVERTICAL );
  sizer->Add(s_sizer, 0 );

  // Class name
  s_sizer = new wxBoxSizer( wxHORIZONTAL );
  s_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Class name")), 1, wxEXPAND );
  s_sizer->Add( m_class_name, 0, wxEXPAND );
  sizer->Add( s_sizer, 1, wxEXPAND | wxALL, 5 );

  // Name
  s_sizer = new wxBoxSizer( wxHORIZONTAL );
  s_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Name")), 1, wxEXPAND );
  s_sizer->Add( m_name, 1, wxEXPAND );  
  sizer->Add( s_sizer, 1, wxEXPAND | wxALL, 5 );
  
  // Tag
  s_sizer = new wxBoxSizer( wxHORIZONTAL );
  s_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Tag")), 1, wxEXPAND );
  s_sizer->Add( m_tag, 1, wxEXPAND );
  sizer->Add( s_sizer, 1, wxEXPAND | wxALL, 5 );

  // Dialog
  sizer->Add( CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0,
              wxALL | wxCENTER, 5 );

  SetSizer(sizer);
} // layer_properties_frame::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "OK" button.
 * \param event This event occured.
 */
void bf::layer_properties_frame::on_ok( wxCommandEvent& WXUNUSED(event) )
{
  EndModal(wxID_OK);
} // layer_properties_frame::on_ok()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "Fit level" checkbox.
 * \param event This event occured.
 */
void bf::layer_properties_frame::on_fit_level_size( wxCommandEvent& event )
{
  m_height->Enable( !event.IsChecked() );
  m_width->Enable( !event.IsChecked() );
} // layer_properties_frame::on_ok()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::layer_properties_frame, wxDialog)
  EVT_BUTTON( wxID_OK, bf::layer_properties_frame::on_ok )
  EVT_CHECKBOX( bf::layer_properties_frame::IDC_FIT_LEVEL_SIZE,
                bf::layer_properties_frame::on_fit_level_size )
END_EVENT_TABLE()
