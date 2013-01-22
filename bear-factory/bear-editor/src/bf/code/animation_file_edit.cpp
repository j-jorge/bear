/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::animation_file_edit class.
 * \author Julien Jorge
 */
#include "bf/animation_file_edit.hpp"

#include "bf/animation_view_ctrl.hpp"
#include "bf/bitmap_rendering_attributes_edit.hpp"
#include "bf/path_configuration.hpp"
#include "bf/wx_facilities.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this window.
 * \param v The initial animation.
 */
bf::animation_file_edit::animation_file_edit
( wxWindow& parent, const animation_file_type& v )
  : wxPanel(&parent, wxID_ANY), base_edit<animation_file_type>(v)
{
  create_controls();
  value_updated();
  Fit();
} // animation_file_edit::animation_file_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the displayed value is correct and, if it is, set the
 *        value according to the display.
 */
bool bf::animation_file_edit::validate()
{
  if ( m_rendering_attributes->validate() )
    {
      set_value(make_animation_file());
      return true;
    }
  else
    return false;
} // animation_file_edit::validate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the animation_file corresponding to the values in the control.
 */
bf::animation_file_type bf::animation_file_edit::make_animation_file() const
{
  animation_file_type result;

  if ( m_rendering_attributes->validate() )
    {
      result.set_path( wx_to_std_string(m_path_text->GetValue()) );
      result.assign(m_rendering_attributes->get_value());
    }

  return result;
} // animation_file_edit::make_animation_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after changing the value by set_value().
 */
void bf::animation_file_edit::value_updated()
{
  m_rendering_attributes->set_value( get_value() );
  fill_controls();
} // animation_file_edit::value_updated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the controls with the values of the animation_file.
 */
void bf::animation_file_edit::fill_controls()
{
  m_rendering_attributes->set_value( get_value() );
  m_path_text->SetValue( std_to_wx_string(get_value().get_path()) );
  animation_view_load();
} // animation_file_edit::fill_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::animation_file_edit::create_controls()
{
  m_rendering_attributes =
    new bitmap_rendering_attributes_edit(*this, get_value());

  m_path_text = new wxTextCtrl( this, wxID_ANY );

  m_animation_view = new animation_view_ctrl(*this);

  create_sizer_controls();
  fill_controls();
} // animation_file_edit::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::animation_file_edit::create_sizer_controls()
{
  wxBoxSizer* v_sizer = new wxBoxSizer( wxVERTICAL );
  wxBoxSizer* h_sizer = new wxBoxSizer( wxHORIZONTAL );

  v_sizer->Add( m_animation_view, 1, wxEXPAND );
  v_sizer->Add( new wxButton(this, wxID_REFRESH), 0, wxEXPAND );

  h_sizer->Add( v_sizer, 1, wxEXPAND | wxALL, 5 );

  v_sizer = new wxBoxSizer( wxVERTICAL );
  v_sizer->Add( m_rendering_attributes, 0, wxEXPAND );
  v_sizer->Add( create_path_sizer(), 0, wxEXPAND );

  h_sizer->Add( v_sizer, 1, wxEXPAND | wxALL, 5 );

  SetSizer(h_sizer);
} // animation_file_edit::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and the sizer for the image name.
 */
wxSizer* bf::animation_file_edit::create_path_sizer()
{
  wxBoxSizer* result = new wxBoxSizer( wxHORIZONTAL );

  wxButton* path_browse = new wxButton
    ( this, IDC_BROWSE_ANIMATION, wxT("..."),
      wxDefaultPosition, wxSize(30, -1) );

  result->Add
    ( new wxStaticText(this, wxID_ANY, _("Animation:")),
      0, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  result->Add( m_path_text, 1, wxEXPAND | wxALL, 5 );
  result->Add( path_browse, 0, wxALL, 5 );

  return result;
} // animation_file_edit::create_path_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the animation in the view by loading the animation file.
 */
void bf::animation_file_edit::animation_view_load()
{
  animation anim = get_value().get_animation();
  m_animation_view->set_animation( anim );
} // animation_file_edit::animation_view_load()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the button to select an animation.
 * \param event This event occured.
 */
void bf::animation_file_edit::on_browse_animation
( wxCommandEvent& WXUNUSED(event) )
{
  std::string p = wx_to_std_string(m_path_text->GetValue());
  path_configuration::get_instance().get_full_path(p);

  wxFileDialog dlg
    ( this, _("Choose a file"), wxEmptyString, std_to_wx_string(p),
      _("Compiled animation (*.canim)|*.canim"),
      wxFD_DEFAULT_STYLE | wxFD_FILE_MUST_EXIST );

  if (dlg.ShowModal() == wxID_OK)
    {
      std::string new_p = wx_to_std_string( dlg.GetPath() );
      path_configuration::get_instance().get_relative_path(new_p);

      m_path_text->SetValue( std_to_wx_string(new_p) );
      animation_file_type v( get_value() );
      v.set_path(new_p);
      set_value(v);

      fill_controls();
    }
} // animation_file_edit::on_browse_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "Refresh" button.
 * \param event This event occured.
 */
void bf::animation_file_edit::on_refresh( wxCommandEvent& WXUNUSED(event) )
{
  validate();
  animation_view_load();
} // animation_file_edit::on_refresh()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::animation_file_edit, wxPanel)
  EVT_BUTTON( bf::animation_file_edit::IDC_BROWSE_ANIMATION,
              bf::animation_file_edit::on_browse_animation )
  EVT_BUTTON( wxID_REFRESH, bf::animation_file_edit::on_refresh )
END_EVENT_TABLE()
