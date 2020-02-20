/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::spin_ctrl class.
 * \author Julien Jorge
 */

#include <sstream>
#include "bf/wx_facilities.hpp"

template<typename T>
const wxEventType bf::spin_event<T>::value_change_event_type = wxNewEventType();

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
template<typename T>
bf::spin_event<T>::spin_event( wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id)
{

} // spin_event::spin_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param value The value of the spin_ctrl.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
template<typename T>
bf::spin_event<T>::spin_event( T value, wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id), m_value(value)
{

} // spin_event::spin_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The event to copy from.
 */
template<typename T>
bf::spin_event<T>::spin_event( const spin_event<T>& that )
  : wxNotifyEvent(that), m_value(that.m_value)
{

} // spin_event::spin_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
template<typename T>
wxEvent* bf::spin_event<T>::Clone() const
{
  return new spin_event<T>(*this);
} // spin_event::Clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of the spin_ctrl.
 */
template<typename T>
T bf::spin_event<T>::get_value() const
{
  return m_value;
} // spin_event::get_value()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<typename T>
bf::spin_ctrl<T>::spin_ctrl
( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
  long style, value_type min, value_type max, value_type initial,
  value_type step, const wxString& name )
  : super( parent, id, pos, size, style | wxTAB_TRAVERSAL ), m_min(min),
    m_max( std::max(min, max) ), m_value(initial), m_step(step)
{
  super::SetName(name);

  CreateControls();
  SetValue(initial);
  DoValueToText();
} // spin_ctrl::spin_ctrl()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the minimum and the maximum values.
 * \param min The minimum value.
 * \param max The maximum value.
 */
template<typename T>
void bf::spin_ctrl<T>::SetRange( value_type min, value_type max )
{
  m_min = min;
  m_max = std::max(m_min, max);
} // spin_ctrl::SetRange()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the step of the spin button.
 * \param s The new step.
 */
template<typename T>
void bf::spin_ctrl<T>::SetStep( value_type s )
{
  m_step = s;
} // spin_ctrl::SetStep()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the current value.
 * \param v The new value.
 */
template<typename T>
void bf::spin_ctrl<T>::SetValue( value_type v )
{
  BoundValue(v);
  ValueToText();
} // spin_ctrl::SetValue()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current value.
 */
template<typename T>
typename bf::spin_ctrl<T>::value_type bf::spin_ctrl<T>::GetValue() const
{
  return m_value;
} // spin_ctrl::GetValue()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the minimum allowed value.
 */
template<typename T>
typename bf::spin_ctrl<T>::value_type bf::spin_ctrl<T>::GetMin() const
{
  return m_min;
} // spin_ctrl::GetMin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the maximum allowed value.
 */
template<typename T>
typename bf::spin_ctrl<T>::value_type bf::spin_ctrl<T>::GetMax() const
{
  return m_max;
} // spin_ctrl::GetMax()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value displayed in the control, not necessary the current
 *        value and maybe even not of type T.
 */
template<typename T>
wxString bf::spin_ctrl<T>::GetValueText() const
{
  return m_text->GetValue();
} // spin_ctrl::GetValueText()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the current value and bound it to stay in the interval.
 * \param v The new value.
 */
template<typename T>
void bf::spin_ctrl<T>::BoundValue( value_type v )
{
  if ( v != m_value )
    {
      if ( v < m_min )
        m_value = m_min;
      else if ( v > m_max )
        m_value = m_max;
      else
        m_value = v;
    }
} // spin_ctrl::BoundValue()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a string representation of the value in the text control.
 */
template<typename T>
void bf::spin_ctrl<T>::ValueToText()
{
  value_type v;
  std::istringstream iss( wx_to_std_string(m_text->GetValue()) );
  bool update_text(false);

  if ( iss >> v )
    if ( iss.rdbuf()->in_avail() == 0 )
      update_text = ( v != m_value );

  if ( update_text )
    DoValueToText();
} // spin_ctrl::ValueToText()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a string representation of the value in the text control.
 */
template<typename T>
void bf::spin_ctrl<T>::DoValueToText()
{
  std::ostringstream oss;
  oss << m_value;

  m_text->ChangeValue( std_to_wx_string(oss.str()) );
} // spin_ctrl::DoValueToText()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sub controls.
 */
template<typename T>
void bf::spin_ctrl<T>::CreateControls()
{
  m_text =
    new wxTextCtrl
    ( this, wxID_ANY, wxT(" "), wxDefaultPosition, wxDefaultSize );
  m_spin = new wxSpinButton( this, wxID_ANY );

  m_spin->SetRange( 1, 3 );
  m_spin->SetValue( 2 );

  wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

  sizer->Add( m_text, 1, wxEXPAND | wxALL, 0 );
  sizer->Add( m_spin, 0, wxALL, 0 );

  this->SetSizer( sizer );

  this->Connect( m_spin->GetId(), wxEVT_SCROLL_LINEUP,
                 wxSpinEventHandler(self_type::OnSpinUp) );
  this->Connect( m_spin->GetId(), wxEVT_SCROLL_LINEDOWN,
                 wxSpinEventHandler(self_type::OnSpinDown) );

  this->Connect( m_text->GetId(), wxEVT_COMMAND_TEXT_UPDATED,
                 wxCommandEventHandler(self_type::OnChange) );
} // spin_ctrl::CreateControls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Increase the value.
 */
template<typename T>
void bf::spin_ctrl<T>::Up()
{
  if ( m_max - m_value < m_step )
    SetValue(m_max);
  else
    SetValue(m_value + m_step);
} // spin_ctrl::Up()

/*----------------------------------------------------------------------------*/
/**
 * \brief Decrease the value.
 */
template<typename T>
void bf::spin_ctrl<T>::Down()
{
  if ( m_value - m_min < m_step )
    SetValue(m_min);
  else
    SetValue(m_value - m_step);
} // spin_ctrl::Down()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send the event value_change.
 */
template<typename T>
void bf::spin_ctrl<T>::SendEvent()
{
  spin_event<T> event
    ( m_value, spin_event<T>::value_change_event_type, GetId() );
  event.SetEventObject(this);
  ProcessEvent(event);
} // spin_ctrl::SendEvent()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "up" arrow.
 * \param event The event that occured.
 */
template<typename T>
void bf::spin_ctrl<T>::OnSpinUp( wxSpinEvent& event )
{
  Up();
  SendEvent();
  event.Veto();
} // spin_ctrl::OnSpinUp()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "down" arrow.
 * \param event The event that occured.
 */
template<typename T>
void bf::spin_ctrl<T>::OnSpinDown( wxSpinEvent& event )
{
  Down();
  SendEvent();
  event.Veto();
} // spin_ctrl::OnSpinDown()

/*----------------------------------------------------------------------------*/
/**
 * \brief The text has been changed.
 * \param event The event that occured.
 */
template<typename T>
void bf::spin_ctrl<T>::OnChange( wxCommandEvent& WXUNUSED(event) )
{
  value_type v;

  std::istringstream iss( wx_to_std_string(m_text->GetValue()) );

  if ( iss >> v )
    if ( iss.rdbuf()->in_avail() == 0 )
      if ( v != m_value )
        {
          BoundValue(v);
          SendEvent();
        }
} // spin_ctrl::OnChange()
