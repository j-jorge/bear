/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::item_reference_edit class.
 * \author Julien Jorge
 */
#include "bf/item_reference_edit.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The windows owning this one.
 * \param choices The valid choices of the value.
 * \param v The initial value.
 */
bf::item_reference_edit::item_reference_edit
( wxWindow& parent, const wxArrayString& choices, const value_type& v )
  : simple_edit<item_reference_type>(v),
    wxComboBox( &parent, IDC_PATTERN_TEXT, wxEmptyString, wxDefaultPosition,
                wxDefaultSize, choices ),
    m_choices(choices)
{
  value_updated();
} // item_reference_edit::item_reference_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the displayed value is correct and, if it is, set the
 *        value according to the display.
 */
bool bf::item_reference_edit::validate()
{
  if ( m_choices.Index( this->GetValue() ) == wxNOT_FOUND )
    return false;
  else
    return value_from_string( GetValue() );
} // item_reference_edit::validate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when the value has been changed, to update the display.
 */
void bf::item_reference_edit::value_updated()
{
  bool found = false;
  wxString val = value_to_string();
  unsigned int i=0;

  while ( (i!=GetCount()) && !found )
    if ( val == GetString(i) )
      found = true;
    else
      ++i;

  if ( found )
    SetSelection(i);
  else
    {
      SetSelection(wxNOT_FOUND);
      Clear();
      Append(m_choices);
    }
  
  SetValue( val );
} // item_reference_edit::value_updated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the list of id that match the pattern.
 */
void bf::item_reference_edit::fill_id_list()
{
  wxString pat( GetValue() );

  wxArrayString::const_iterator it = m_choices.begin();
  wxArrayString::const_iterator eit = m_choices.end();

  const wxArrayString old_choices( GetStrings() );

  Clear();

  if ( pat.IsEmpty() )
    pat = wxT("*");

  if ( (pat[0] != wxT('*')) && (pat[0] != wxT('?')) )
    pat = wxT("*") + pat;

  if ( (pat[pat.length() - 1] != wxT('*'))
       && (pat[pat.length() - 1] != wxT('?')) )
    pat += wxT("*");

  for (; it!=eit; ++it)
    if ( it->Matches( pat ) )
      Append( *it );

  if ( GetCount() == 1 )
    {
      SetSelection(0);
      m_last_valid_value = GetValue();
    }
  else if ( GetCount() == 0 )
    {
      Append( old_choices );
      SetValue( m_last_valid_value );
    }
  else
    m_last_valid_value = GetValue();
} // item_reference_edit::fill_id_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief The text of the pattern has changed.
 * \param event The event.
 */
void bf::item_reference_edit::on_pattern_change
( wxCommandEvent& WXUNUSED(event) )
{
  fill_id_list();
} // item_reference_edit::on_pattern_change()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf:: item_reference_edit, wxControl)
  EVT_TEXT( bf::item_reference_edit::IDC_PATTERN_TEXT,
            bf::item_reference_edit::on_pattern_change )
END_EVENT_TABLE()
