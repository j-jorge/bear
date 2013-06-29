/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::frame_edit class.
 * \author Sebastien Angibaud
 */
#include "bf/frame_edit.hpp"

#include "bf/sprite_edit.hpp"
#include "bf/spin_ctrl.hpp"
#include "bf/free_edit.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this window.
 * \param frame The initial frame.
 */
bf::frame_edit::frame_edit( wxWindow& parent, const animation_frame& frame )
  : wxDialog( &parent, wxID_ANY, _("Frame"),
              wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ), m_frame(frame)
{
  create_controls();
} // frame_edit::frame_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Validate the parameters.
 */
void bf::frame_edit::validate()
{
  m_sprite->validate();
  m_duration->validate();
  m_frame.set_sprite(m_sprite->get_value());
  m_frame.set_duration(m_duration->get_value().get_value());
} // frame_edit::validate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::frame_edit::create_controls()
{
  m_duration =
    new free_edit<real_type>( *this, real_type(m_frame.get_duration()) );
  m_sprite = new sprite_edit( *this, m_frame.get_sprite() );

  create_sizer_controls();

  Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
           wxCommandEventHandler(frame_edit::on_ok) );
} // frame_edit::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::frame_edit::create_sizer_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  sizer->Add( m_sprite, 1, wxEXPAND );

  wxBoxSizer* h_sizer = new wxBoxSizer( wxHORIZONTAL );
  h_sizer->Add( new wxStaticText(this, wxID_ANY, _("Duration:")),
                1, wxALIGN_CENTRE_VERTICAL | wxALL, 0 );
  h_sizer->Add( m_duration, 0, wxALL, 0 );
  sizer->Add( h_sizer, 0, wxALL, 0 );
  sizer->Add( CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0,
              wxALL | wxALIGN_CENTRE_HORIZONTAL, 0 );
  SetSizer(sizer);
  Fit();
} // frame_edit::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the frame.
 */
const bf::animation_frame& bf::frame_edit::get_frame() const
{
  return m_frame;
} // frame_edit::get_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Action performed when clicking the "OK" button.
 * \param event This event occured.
 */
void bf::frame_edit::on_ok( wxCommandEvent& WXUNUSED(event) )
{
  validate();
  EndModal(wxID_OK);
} // frame_edit::on_ok()
