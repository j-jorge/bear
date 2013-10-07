/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::mark_properties_frame class.
 * \author Sébastien Angibaud
 */
#include "bf/mark_properties_frame.hpp"

#include "bf/action.hpp"
#include "bf/any_animation_edit.hpp"
#include "bf/value_editor_dialog.hpp"
#include "bf/workspace_environment.hpp"
#include "bf/wx_facilities.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this window.
 * \param a The edited action.
 * \param env The workspace environment to use.
 */
bf::mark_properties_frame::mark_properties_frame
( wxWindow* parent, const action* a, workspace_environment* env )
  : wxDialog(parent, wxID_ANY, wxString(_("Mark properties"))), m_action(a),
    m_apply_angle_to_animation(false), m_pause_animation_when_hidden(false),
    m_reset_animation_with_action(true), m_workspace(env)
{
  create_controls();
  Fit();
} // mark_properties_frame::mark_properties_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the mark.
 */
const std::string& bf::mark_properties_frame::get_mark_name() const
{
  return m_mark_label;
} // mark_properties_frame::get_mark_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the animation of the mark.
 */
const bf::any_animation&
bf::mark_properties_frame::get_mark_animation() const
{
  return m_mark_animation;
} // mark_properties_frame::get_mark_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the angle applied to the mark is also applied to the
 *        animation.
 */
bool bf::mark_properties_frame::apply_angle_to_animation() const
{
  return m_apply_angle_to_animation;
} // mark_properties_frame::apply_angle_to_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the animation is paused when the mark is hidden.
 */
bool bf::mark_properties_frame::pause_animation_when_hidden() const
{
  return m_pause_animation_when_hidden;
} // mark_properties_frame::pause_animation_when_hidden()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the animation must be reset when the action begins.
 */
bool bf::mark_properties_frame::reset_animation_with_action() const
{
  return m_reset_animation_with_action;
} // mark_properties_frame::reset_animation_with_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the properties of the mark.
 * \param a The mark from which we take the info.
 */
void bf::mark_properties_frame::fill_from( const mark& a )
{
  m_mark_label = a.get_label();
  m_mark_animation = a.get_animation();
  m_mark_animation.reload(m_workspace);
  m_apply_angle_to_animation = a.apply_angle_to_animation();
  m_pause_animation_when_hidden = a.pause_animation_when_hidden();
  m_reset_animation_with_action = a.reset_animation_with_action();

  fill_controls();
} // mark_properties_frame::set_mark_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the controls with the values of the mark.
 */
void bf::mark_properties_frame::fill_controls()
{
  m_mark_label_box->SetValue( std_to_wx_string(m_mark_label) );
  m_apply_angle_box->SetValue( m_apply_angle_to_animation );
  m_pause_when_hidden_box->SetValue( m_pause_animation_when_hidden );
  m_reset_with_action_box->SetValue( m_reset_animation_with_action );
} // mark_properties_frame::fill_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::mark_properties_frame::create_controls()
{
  create_member_controls();
  create_sizer_controls();
  fill_controls();
} // mark_properties_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief for which we keep a reference.
 */
void bf::mark_properties_frame::create_member_controls()
{
  m_mark_label_box = new wxTextCtrl( this, wxID_ANY );
  m_edit_animation = new wxButton(this, IDC_EDIT_ANIMATION, _("Edit"));
  m_apply_angle_box = new wxCheckBox
    ( this, wxID_ANY, _("Apply the angle of the mark to the animation.") );
  m_pause_when_hidden_box = new wxCheckBox
    ( this, wxID_ANY, _("Pause the animation when the mark is hidden.") );
  m_reset_with_action_box = new wxCheckBox
    ( this, wxID_ANY, _("Reset the animation when the action begins.") );
} // mark_properties_frame::create_member_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::mark_properties_frame::create_sizer_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  wxBoxSizer* s_sizer = new wxBoxSizer( wxHORIZONTAL );

  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Mark name")),
                1, wxEXPAND | wxALL, 5 );
  s_sizer->Add( m_mark_label_box, 1, wxEXPAND );
  sizer->Add( s_sizer, 0,
              wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxALIGN_LEFT, 10 );

  s_sizer = new wxBoxSizer( wxHORIZONTAL );

  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Mark animation")),
                1, wxEXPAND | wxALL, 5 );
  s_sizer->Add( m_edit_animation, 3, wxEXPAND );
  sizer->Add( s_sizer, 0, wxEXPAND | wxALL, 10 );

  sizer->Add( m_apply_angle_box, 0, wxALL, 10 );
  sizer->Add( m_pause_when_hidden_box, 0, wxALL, 10 );
  sizer->Add( m_reset_with_action_box, 0, wxALL, 10 );

  sizer->Add( CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0,
              wxALL | wxCENTER, 5 );

  SetSizer(sizer);
} // mark_properties_frame::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "OK" button.
 * \param event This event occured.
 */
void bf::mark_properties_frame::on_ok(wxCommandEvent& WXUNUSED(event))
{
  if ( m_mark_label_box->GetValue().empty() )
    {
      wxMessageDialog dlg
        ( this, _("The name field is empty."), _("Bad name"), wxID_OK );
      dlg.ShowModal();
    }
  else if ( ( wx_to_std_string(m_mark_label_box->GetValue())
              != m_mark_label ) &&
            ( m_action->has_mark_label
              ( wx_to_std_string( m_mark_label_box->GetValue() ) ) ) )
    {
      wxMessageDialog dlg
        ( this, _("This name is already used."), _("Bad name"), wxID_OK );
      dlg.ShowModal();
    }
  else
    {
      m_mark_label = wx_to_std_string( m_mark_label_box->GetValue() );
      m_apply_angle_to_animation = m_apply_angle_box->GetValue();
      m_pause_animation_when_hidden = m_pause_when_hidden_box->GetValue();
      m_reset_animation_with_action = m_reset_with_action_box->GetValue();

      EndModal(wxID_OK);
    }
} // mark_properties_frame::on_ok()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "Edit" button of the animation.
 * \param event This event occured.
 */
void bf::mark_properties_frame::on_edit_animation
(wxCommandEvent& WXUNUSED(event))
{
  value_editor_dialog<any_animation_edit> dlg
    (*this, _("Animation"), m_mark_animation, m_workspace);

  if ( dlg.ShowModal() == wxID_OK )
    m_mark_animation = dlg.get_value();
} // mark_properties_frame::on_edit_animation()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::mark_properties_frame, wxDialog)
  EVT_BUTTON( wxID_OK, bf::mark_properties_frame::on_ok )
  EVT_BUTTON( bf::mark_properties_frame::IDC_EDIT_ANIMATION,
              bf::mark_properties_frame::on_edit_animation )
END_EVENT_TABLE()
