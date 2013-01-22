/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::bool_edit class.
 * \author Julien Jorge
 */
#include "bf/bool_edit.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The windows owning this one.
 * \param v The initial value.
 */
bf::bool_edit::bool_edit( wxWindow& parent, const value_type& v )
  : base_edit<bool_type>(v),
    wxCheckBox(&parent, wxID_ANY, _("'True' if checked"))
{
  init();
} // bool_edit::bool_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the displayed value is correct and, if it is, set the
 *        value according to the display.
 */
bool bf::bool_edit::validate()
{
  set_value( bool_type(GetValue()) );

  return true;
} // bool_edit::validate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the control.
 */
void bf::bool_edit::init()
{
  Connect( GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,
           wxCommandEventHandler(bool_edit::on_change) );
  value_updated();
} // bool_edit::init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when the value has been changed, to update the display.
 */
void bf::bool_edit::value_updated()
{
  SetValue( get_value().get_value() );
} // bool_edit::value_updated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event handler for a change in the text.
 * \param event The event.
 */
void bf::bool_edit::on_change( wxCommandEvent& WXUNUSED(event) )
{
  validate();
} // bool_edit::on_change()
