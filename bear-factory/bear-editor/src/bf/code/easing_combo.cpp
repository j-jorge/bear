/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::easing_combo functions.
 * \author Julien Jorge
 */
#include "bf/easing_combo.hpp"

#include "bf/easing_combo_popup.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a combo for the easing functions.
 * \param parent The parent control of the combo.
 */
wxComboCtrl* bf::create_easing_combo( wxWindow* parent )
{
  wxComboCtrl* result =
    new wxComboCtrl
    ( parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
      wxCB_READONLY );

  initialize_easing_combo( *result );

  return result;
} // create_easing_combo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initializes a combo for the easing functions.
 * \param combo The he combo to initialize.
 */
void bf::initialize_easing_combo( wxComboCtrl& combo )
{
  combo.SetPopupControl( new easing_combo_popup );
  combo.SetPopupMinWidth( 400 );
} // initialize_easing_combo()
