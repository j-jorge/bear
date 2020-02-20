/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::easing_edit class.
 * \author Julien Jorge
 */
#include "bf/easing_edit.hpp"

#include "bf/easing_combo.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The windows owning this one.
 * \param v The initial value.
 */
bf::easing_edit::easing_edit
( wxWindow& parent, const value_type& v )
  : simple_edit(v),
    wxComboCtrl( &parent, wxID_ANY, wxEmptyString, wxDefaultPosition,
                 wxDefaultSize, wxCB_READONLY )
{
  initialize_easing_combo( *this );

  value_updated();
} // easing_edit::easing_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the displayed value is correct and, if it is, set the
 *        value according to the display.
 */
bool bf::easing_edit::validate()
{
  bear::easing e;
  e.from_string( wx_to_std_string( GetPopupControl()->GetStringValue() ) );

  easing_type v;
  v.set_value( e );

  set_value( v );

  return true;
} // easing_edit::validate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when the value has been changed, to update the display.
 */
void bf::easing_edit::value_updated()
{
  SetValue( value_to_string() );
} // easing_edit::value_updated()
