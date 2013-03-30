/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A combobox containing all the easing functions.
 * \author Julien Jorge
 */
#ifndef __BF_EASING_COMBO_HPP__
#define __BF_EASING_COMBO_HPP__

#include <wx/combo.h>

namespace bf
{
  wxComboCtrl* create_easing_combo( wxWindow* parent );
  void initialize_easing_combo( wxComboCtrl& combo );

} // namespace bf

#endif // __BF_EASING_COMBO_HPP__
