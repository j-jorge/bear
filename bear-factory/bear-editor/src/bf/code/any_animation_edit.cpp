/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::any_animation_edit class.
 * \author Julien Jorge
 */
#include "bf/any_animation_edit.hpp"

#include "bf/animation_edit.hpp"
#include "bf/animation_file_edit.hpp"

#include "bf/wx_facilities.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this window.
 * \param env The workspace environment to use.
 * \param v The initial animation.
 */
bf::any_animation_edit::any_animation_edit
( wxWindow& parent, workspace_environment& env, const any_animation& v )
  : wxPanel(&parent, wxID_ANY), base_edit<any_animation>(v), 
    m_workspace(env)
{
  create_controls();
  value_updated();
  Fit();
} // any_animation_edit::any_animation_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the displayed value is correct and, if it is, set the
 *        value according to the display.
 */
bool bf::any_animation_edit::validate()
{
  bool result = false;
  any_animation anim;

  switch( get_visible_content_type() )
    {
    case any_animation::content_animation:
      if ( m_animation_edit->validate() )
        {
          anim.set_animation( m_animation_edit->get_value() );
          result = true;
        }
      break;
    case any_animation::content_file:
      if ( m_animation_file_edit->validate() )
        {
          anim.set_animation_file( m_animation_file_edit->get_value() );
          result = true;
        }
      break;
    }

  if ( result )
    set_value( anim );

  return result;
} // any_animation_edit::validate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after changing the value by set_value().
 */
void bf::any_animation_edit::value_updated()
{
  fill_controls();
} // any_animation_edit::value_updated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the controls with the values of the any_animation.
 */
void bf::any_animation_edit::fill_controls()
{
  switch( get_value().get_content_type() )
    {
    case any_animation::content_animation:
      m_animation_edit->set_value( get_value().get_animation() );
      GetSizer()->Hide(m_animation_file_edit);
      GetSizer()->Show(m_animation_edit);
      break;
    case any_animation::content_file:
      m_animation_file_edit->set_value( get_value().get_animation_file() );
      GetSizer()->Hide(m_animation_edit);
      GetSizer()->Show(m_animation_file_edit);
      break;
    }

  m_content_type->SetStringSelection
    ( std_to_wx_string
      ( any_animation::content_to_string(get_value().get_content_type()) ) );

  GetSizer()->Layout();
} // any_animation_edit::fill_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the content type displayed to the user.
 */
bf::any_animation::content_type
bf::any_animation_edit::get_visible_content_type() const
{
  return any_animation::string_to_content
    ( wx_to_std_string( m_content_type->GetStringSelection() ) );
} // any_animation_edit::get_visible_content_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::any_animation_edit::create_controls()
{
  wxArrayString choices;

  choices.Add
    ( std_to_wx_string
      ( any_animation::content_to_string(any_animation::content_animation) ) );
  choices.Add
    ( std_to_wx_string
      ( any_animation::content_to_string(any_animation::content_file) ) );

  m_content_type =
    new wxChoice
    ( this, IDC_CONTENT_TYPE, wxDefaultPosition, wxDefaultSize, choices );

  m_content_type->SetStringSelection
    ( std_to_wx_string
      ( any_animation::content_to_string(get_value().get_content_type()) ) );

  m_animation_edit = new animation_edit(*this, m_workspace);
  m_animation_file_edit = new animation_file_edit(*this, m_workspace);

  create_sizer_controls();
  fill_controls();
} // any_animation_edit::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::any_animation_edit::create_sizer_controls()
{
  wxBoxSizer* v_sizer = new wxBoxSizer( wxVERTICAL );
  wxBoxSizer* h_sizer = new wxBoxSizer( wxHORIZONTAL );

  h_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Content type")), 0, wxEXPAND );
  h_sizer->Add( m_content_type, 1, wxEXPAND );

  v_sizer->Add( h_sizer, 0, wxEXPAND );
  v_sizer->Add( m_animation_edit, 1, wxEXPAND );
  v_sizer->Add( m_animation_file_edit, 1, wxEXPAND );

  SetSizer(v_sizer);
} // any_animation_edit::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user changed the content type of the animation.
 * \param event This event occured.
 */
void bf::any_animation_edit::on_switch_content_type
( wxCommandEvent& WXUNUSED(event) )
{
  any_animation::content_type c = get_visible_content_type();

  if ( c != get_value().get_content_type() )
    {
      any_animation anim = get_value();
      anim.switch_to(c);
      set_value(anim);
    }
} // any_animation_edit::on_switch_content_type()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::any_animation_edit, wxPanel)
  EVT_CHOICE( bf::any_animation_edit::IDC_CONTENT_TYPE,
              bf::any_animation_edit::on_switch_content_type )
END_EVENT_TABLE()
