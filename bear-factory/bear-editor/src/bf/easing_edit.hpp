/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The window showing the properties of an easing.
 * \author Julien Jorge
 */
#ifndef __BF_EASING_EDIT_HPP__
#define __BF_EASING_EDIT_HPP__

#include "bf/default_value.hpp"
#include "bf/libeditor_export.hpp"

#include "bf/simple_edit.hpp"
#include "bf/custom_type.hpp"

#include <wx/combo.h>

namespace bf
{
  /**
   * \brief The window showing the properties of an easing function.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT easing_edit:
    public simple_edit<easing_type>,
    public wxComboCtrl
  {
  public:
    easing_edit
    ( wxWindow& parent,
      const easing_type& s = default_value<easing_type>::get() );

    bool validate();

  private:
    void value_updated();

  }; // class easing_edit
} // namespace bf

#endif // __BF_EASING_EDIT_HPP__
