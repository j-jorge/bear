/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_properties_frame class.
 * \author Sébastien Angibaud
 */
#include "bf/action_properties_frame.hpp"

#include "bf/wx_facilities.hpp"
#include "bf/gui_model.hpp"
#include "bf/sound_frame.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this window.
 * \param m The model edited.
 */
bf::action_properties_frame::action_properties_frame
( wxWindow* parent, const gui_model* m )
  : wxDialog(parent, wxID_ANY, wxString(_("Action properties"))), m_model(m),
    m_duration(1)
{
  create_controls();
  Fit();
} // action_properties_frame::action_properties_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the duration of the action.
 */
double bf::action_properties_frame::get_action_duration() const
{
  return m_duration;
} // action_properties_frame::get_action_duration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the action.
 */
const std::string& bf::action_properties_frame::get_action_name() const
{
  return m_action_name;
} // action_properties_frame::get_action_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sound played during the action.
 */
const bf::sound_description& bf::action_properties_frame::get_sound() const
{
  return m_sound;
} // action_properties_frame::get_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the action to start after this one.
 */
const std::string&  bf::action_properties_frame::get_auto_next() const
{
  return m_auto_next;
} // action_properties_frame::get_auto_next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the properties of the action.
 * \param a The action from which we take the info.
 *
 * The new duration is min(a.get_duration(), 0).
 */
void bf::action_properties_frame::fill_from( const action& a )
{
  m_duration = a.get_duration();
  if ( m_duration < 0 )
    m_duration = 0;

  m_action_name = a.get_name();
  m_sound = a.get_sound();
  m_auto_next = a.get_auto_next();

  fill_controls();
} // action_properties_frame::set_action_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the controls with the values of the action.
 */
void bf::action_properties_frame::fill_controls()
{
  m_duration_spin->SetValue(m_duration);
  m_action_name_box->SetValue( std_to_wx_string(m_action_name) );

  m_sound_frame->set_sound_description( m_sound );
  m_auto_next_text->SetValue(std_to_wx_string(m_auto_next));
} // action_properties_frame::fill_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::action_properties_frame::create_controls()
{
  create_member_controls();
  create_sizer_controls();
  fill_controls();
} // action_properties_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief for which we keep a reference.
 */
void bf::action_properties_frame::create_member_controls()
{
  m_duration_spin = new spin_ctrl<double>
    ( this, ID_DURATION_SPIN, wxDefaultPosition,
      wxDefaultSize, 0, 0,
      std::numeric_limits<double>::infinity(), 0, 0.1 );
  m_action_name_box = new wxTextCtrl( this, wxID_ANY );
  m_auto_next_text = new wxTextCtrl( this, wxID_ANY );

  m_sound_frame = new sound_frame( this, wxID_ANY );
} // action_properties_frame::create_member_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::action_properties_frame::create_sizer_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  wxStaticBoxSizer* static_sizer =
    new wxStaticBoxSizer( wxVERTICAL, this, _("Properties") );
  wxBoxSizer* s_sizer = new wxBoxSizer( wxHORIZONTAL );

  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Name:")), 0,
                     wxALIGN_CENTER_VERTICAL );
  s_sizer->Add( m_action_name_box, 1, wxALL );
  s_sizer->AddSpacer(5);
  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Duration:")), 0,
                     wxALIGN_CENTER_VERTICAL );
  s_sizer->Add( m_duration_spin, 1 );
  static_sizer->Add( s_sizer, 0, wxEXPAND );
  s_sizer = new wxBoxSizer( wxHORIZONTAL );
  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Next action:")), 0,
                     wxALIGN_CENTER_VERTICAL );
  s_sizer->Add( m_auto_next_text, 1, wxALL );
  static_sizer->Add( s_sizer, 0, wxEXPAND );
  sizer->Add( static_sizer, 0, wxEXPAND );

  static_sizer = new wxStaticBoxSizer( wxVERTICAL, this, _("Sound") );
  static_sizer->Add( m_sound_frame, 1, wxEXPAND );
  sizer->Add( static_sizer, 0, wxEXPAND );

  sizer->Add( CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxCENTER );

  SetSizer(sizer);
} // action_properties_frame::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "OK" button.
 * \param event This event occured.
 */
void bf::action_properties_frame::on_ok( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_action_name_box->GetValue().empty() )
    {
      wxMessageDialog dlg
        ( this, _("The name field is empty."),
          _("Bad name"), wxID_OK );
      dlg.ShowModal();
    }
  else if ( ( wx_to_std_string(m_action_name_box->GetValue())
              != m_action_name ) &&
            ( m_model->has_action
              ( wx_to_std_string( m_action_name_box->GetValue() ) ) ) )
    {
      wxMessageDialog dlg
        ( this, _("This action already exists."),
          _("Bad name"), wxID_OK );
      dlg.ShowModal();
    }
  else
    {
      m_duration = m_duration_spin->GetValue();
      m_action_name = wx_to_std_string( m_action_name_box->GetValue() );
      m_sound = m_sound_frame->get_sound_description();
      m_auto_next = wx_to_std_string(m_auto_next_text->GetValue());

      EndModal(wxID_OK);
    }
} // action_properties_frame::on_ok()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::action_properties_frame, wxDialog)
  EVT_BUTTON( wxID_OK, bf::action_properties_frame::on_ok )
END_EVENT_TABLE()
