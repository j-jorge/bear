/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::level_properties_frame class.
 * \author Julien Jorge
 */
#include "bf/level_properties_frame.hpp"

#include "bf/level.hpp"
#include "bf/path_configuration.hpp"
#include "bf/wx_facilities.hpp"

#include <limits>
#include <string>

/*----------------------------------------------------------------------------*/
const unsigned int bf::level_properties_frame::s_min_width(800);
const unsigned int bf::level_properties_frame::s_min_height(600);

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this window.
 */
bf::level_properties_frame::level_properties_frame( wxWindow* parent )
  : wxDialog(parent, wxID_ANY, wxString(_("Level properties"))),
    m_width(s_min_width), m_height(s_min_height)
{
  create_controls();
  Fit();
} // level_properties_frame::level_properties_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of the level.
 */
unsigned int bf::level_properties_frame::get_width() const
{
  return m_width;
} // level_properties_frame::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of the level.
 */
unsigned int bf::level_properties_frame::get_height() const
{
  return m_height;
} // level_properties_frame::get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the music of the level.
 */
const std::string& bf::level_properties_frame::get_music() const
{
  return m_music;
} // level_properties_frame::get_music()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the level.
 */
const std::string& bf::level_properties_frame::get_name() const
{
  return m_name;
} // level_properties_frame::get_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the values from an existing level.
 * \param lvl The level in which we take the values.
 */
void bf::level_properties_frame::init_from( const level& lvl )
{
  m_width = lvl.get_width();
  m_height = lvl.get_height();
  m_music = lvl.get_music();
  m_name = lvl.get_name();

  fill_controls();
} // level_properties_frame::init_from()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the controls with the values of the level.
 */
void bf::level_properties_frame::fill_controls()
{
  m_width_text->SetValue( wxString::Format(wxT("%d"), m_width) );
  m_height_text->SetValue( wxString::Format(wxT("%d"), m_height) );
  m_music_text->SetValue( std_to_wx_string(m_music) );
  m_name_text->SetValue( std_to_wx_string(m_name) );
} // level_properties_frame::fill_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::level_properties_frame::create_controls()
{
  create_sizer_controls();
  fill_controls();
} // level_properties_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::level_properties_frame::create_sizer_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  sizer->Add( create_level_sizer(), 0, wxEXPAND );
  sizer->Add( create_music_sizer(), 0, wxEXPAND );
  sizer->Add( create_name_sizer(), 0, wxEXPAND );

  sizer->Add( CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0,
              wxALL | wxCENTER, 5 );

  SetSizer(sizer);
} // level_properties_frame::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and the sizer for the level part.
 */
wxSizer* bf::level_properties_frame::create_level_sizer()
{
  wxStaticBoxSizer* result =
    new wxStaticBoxSizer( wxHORIZONTAL, this, _("Level") );

  m_width_text = new wxSpinCtrl( this, wxID_ANY );
  m_height_text = new wxSpinCtrl( this, wxID_ANY );
  m_width_text->SetRange( s_min_width, std::numeric_limits<int>::max() );
  m_height_text->SetRange( s_min_height, std::numeric_limits<int>::max() );

  result->Add
    ( new wxStaticText(this, wxID_ANY, _("Width:")),
      1, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  result->Add( m_width_text, 0, wxALL, 5 );

  result->Add
    ( new wxStaticText(this, wxID_ANY, _("Height:")),
      1, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  result->Add( m_height_text, 0, wxALL, 5 );

  return result;
} // level_properties_frame::create_level_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and the sizer for the music.
 */
wxSizer* bf::level_properties_frame::create_music_sizer()
{
  wxBoxSizer* result = new wxBoxSizer( wxHORIZONTAL );

  m_music_text = new wxTextCtrl( this, wxID_ANY );
  m_music_browse = new wxButton
    ( this, ID_BROWSE, wxT("..."), wxDefaultPosition, wxSize(30, -1) );

  result->Add
    ( new wxStaticText(this, wxID_ANY, _("Music:")),
      0, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  result->Add( m_music_text, 1, wxEXPAND | wxALL, 5 );
  result->Add( m_music_browse, 0, wxALL, 5 );

  return result;
} // level_properties_frame::create_music_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and the sizer for the name.
 */
wxSizer* bf::level_properties_frame::create_name_sizer()
{
  wxBoxSizer* result = new wxBoxSizer( wxHORIZONTAL );

  m_name_text = new wxTextCtrl( this, wxID_ANY );

  result->Add
    ( new wxStaticText(this, wxID_ANY, _("Name:")),
      0, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  result->Add( m_name_text, 1, wxEXPAND | wxALL, 5 );

  return result;
} // level_properties_frame::create_name_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "OK" button.
 * \param event This event occured.
 */
void bf::level_properties_frame::on_ok( wxCommandEvent& WXUNUSED(event) )
{
  m_width = m_width_text->GetValue();
  m_height = m_height_text->GetValue();
  m_music = wx_to_std_string(m_music_text->GetValue());
  m_name = wx_to_std_string(m_name_text->GetValue());

  EndModal(wxID_OK);
} // level_properties_frame::on_ok()

/*----------------------------------------------------------------------------*/
/**
 * \brief Browse the music.
 * \param event This event occured.
 */
void bf::level_properties_frame::on_browse( wxCommandEvent& WXUNUSED(event) )
{
  std::string p = wx_to_std_string(m_music_text->GetValue());
  wxString full_path;

  if ( path_configuration::get_instance().get_full_path(p) )
    full_path = std_to_wx_string(p);

  wxFileDialog dlg
    (this, _("Choose a music"), wxEmptyString, full_path,
     _("Music files (*.ogg)|*.ogg|All files (*.*)|*"),
     wxFD_OPEN | wxFD_PREVIEW);

  if (dlg.ShowModal() == wxID_OK)
    {
      std::string new_p = wx_to_std_string( dlg.GetPath() );
      path_configuration::get_instance().get_relative_path(new_p);

      m_music_text->SetValue( std_to_wx_string(new_p) );
    }
} // level_properties_frame::on_browse()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::level_properties_frame, wxDialog)
  EVT_BUTTON( wxID_OK, bf::level_properties_frame::on_ok )
  EVT_BUTTON( bf::level_properties_frame::ID_BROWSE,
              bf::level_properties_frame::on_browse )
END_EVENT_TABLE()
