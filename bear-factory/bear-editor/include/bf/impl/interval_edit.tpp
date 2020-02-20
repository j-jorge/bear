/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::interval_edit class.
 * \author Julien Jorge
 */

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The windows owning this one.
 * \param min The minimum valid value.
 * \param max The maximum valid value.
 * \param v The initial value.
 */
template<typename Type>
bf::interval_edit<Type>::interval_edit
( wxWindow& parent, const value_type& min, const value_type& max,
  const value_type& v )
  : super_edit(v),
    super_ctrl( &parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0,
                min.get_value(), max.get_value() )
{
  value_updated();
} // interval_edit::interval_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the displayed value is correct and, if it is, set the
 *        value according to the display.
 */
template<typename Type>
bool bf::interval_edit<Type>::validate()
{
  bool result = false;

  if ( this->value_from_string( this->GetValueText() ) )
    result = (this->get_value().get_value() == this->GetValue() );

  return result;
} // interval_edit::validate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when the value has been changed, to update the display.
 */
template<typename Type>
void bf::interval_edit<Type>::value_updated()
{
  this->SetValue( this->get_value().get_value() );
} // interval_edit::value_updated()
