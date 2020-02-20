/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::set_edit class.
 * \author Julien Jorge
 */

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The windows owning this one.
 * \param choices The valid choices of the value.
 * \param v The initial value.
 */
template<typename Type>
bf::set_edit<Type>::set_edit
( wxWindow& parent, const wxArrayString& choices, const value_type& v )
  : simple_edit<Type>(v),
    wxChoice( &parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices )
{
  CLAW_PRECOND( !choices.IsEmpty() );

  value_updated();
} // set_edit::set_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the displayed value is correct and, if it is, set the
 *        value according to the display.
 */
template<typename Type>
bool bf::set_edit<Type>::validate()
{
  return this->value_from_string( this->GetStringSelection() );
} // set_edit::validate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when the value has been changed, to update the display.
 */
template<typename Type>
void bf::set_edit<Type>::value_updated()
{
  bool found = false;
  wxString val = this->value_to_string();
  unsigned int i=0;

  while ( (i!=this->GetCount()) && !found )
    if ( val == this->GetString(i) )
      found = true;
    else
      ++i;

  if ( found )
    this->SetSelection(i);
  else
    {
      this->SetSelection(0);
      validate();
    }
} // set_edit::value_updated()
