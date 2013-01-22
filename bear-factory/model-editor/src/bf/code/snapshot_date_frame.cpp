/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::snapshot_date_frame class.
 * \author Sébastien Angibaud
 */
#include "bf/snapshot_date_frame.hpp"

#include "bf/action.hpp"
#include "bf/snapshot.hpp"
#include "bf/wx_facilities.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this window.
 * \param a The action edited.
 */
bf::snapshot_date_frame::snapshot_date_frame
( wxWindow* parent, const action* a )
  : wxDialog(parent, wxID_ANY, wxString(_("Snapshot's date"))),
    m_action(a), m_date(0)
{
  create_controls();
  Fit();
} // snapshot_date_frame::snapshot_date_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the date of the snapshot.
 */
double bf::snapshot_date_frame::get_date() const
{
  return m_date;
} // snapshot_date_frame::get_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the date of the snapshot.
 * \param s The snapshot from which we take the info.
 */
void bf::snapshot_date_frame::fill_from( const snapshot* s )
{
  m_date = s->get_date();

  fill_controls();
} // snapshot_date_frame::set_snapshot_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the controls with the values of the snapshot.
 */
void bf::snapshot_date_frame::fill_controls()
{
  m_date_spin->SetValue(m_date);
} // snapshot_date_frame::fill_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::snapshot_date_frame::create_controls()
{
  create_member_controls();
  create_sizer_controls();
  fill_controls();
} // snapshot_date_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief for which we keep a reference.
 */
void bf::snapshot_date_frame::create_member_controls()
{
  m_date_spin = new spin_ctrl<double>
    ( this, wxID_ANY, wxDefaultPosition,
      wxDefaultSize, 0, 0, m_action->get_duration(), 0, 0.1);
} // snapshot_date_frame::create_member_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::snapshot_date_frame::create_sizer_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  // width
  wxBoxSizer* s_sizer = new wxBoxSizer( wxHORIZONTAL );

  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Snapshot's date")),
                1, wxEXPAND | wxALL, 5 );
  s_sizer->Add( m_date_spin, 0, wxEXPAND );
  sizer->Add( s_sizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10 );

  sizer->Add( CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0,
              wxALL | wxCENTER, 5 );

  SetSizer(sizer);
} // snapshot_date_frame::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "OK" button.
 * \param event This event occured.
 */
void bf::snapshot_date_frame::on_ok(wxCommandEvent& WXUNUSED(event))
{
  bool ok = true;

  double value = m_date_spin->GetValue();
  value = (double)((int)((value+0.0005) * 1000)) / 1000;
  m_date_spin->SetValue(value);

  if ( value != m_date )
    if ( m_action->has_snapshot_date( value ) )
      {
        wxMessageDialog dlg
          ( this,
            _("This date is already used. The snapshot at this date is going to"
              " be removed."),
            _("Snapshot at this date"), wxCANCEL | wxOK );
        ok = ( dlg.ShowModal() == wxID_OK );
      }

  if ( ok )
    {
      m_date = value;
      EndModal(wxID_OK);
    }
} // snapshot_date_frame::on_ok()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::snapshot_date_frame, wxDialog)
  EVT_BUTTON( wxID_OK, bf::snapshot_date_frame::on_ok )
END_EVENT_TABLE()
