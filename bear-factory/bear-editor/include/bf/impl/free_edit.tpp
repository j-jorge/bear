/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::free_edit class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this one.
 */
template<typename Type>
bf::free_edit<Type>::free_edit( wxWindow& parent )
  : simple_edit<Type>(default_value<Type>::get()),
    wxTextCtrl( &parent, wxID_ANY )
{
  value_updated();
} // free_edit::free_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this one.
 * \param v The initial value.
 */
template<typename Type>
bf::free_edit<Type>::free_edit( wxWindow& parent, const value_type& v )
  : simple_edit<Type>(v), wxTextCtrl( &parent, wxID_ANY )
{
  value_updated();
} // free_edit::free_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the displayed value is correct and, if it is, set the
 *        value according to the display.
 */
template<typename Type>
bool bf::free_edit<Type>::validate()
{
  return this->value_from_string( this->GetValue() );
} // free_edit::validate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when the value has been changed, to update the display.
 */
template<typename Type>
void bf::free_edit<Type>::value_updated()
{
  this->SetValue( this->value_to_string() );
} // free_edit::value_updated()
