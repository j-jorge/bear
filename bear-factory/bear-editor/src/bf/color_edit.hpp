/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Control for editing a field whose value is a color.
 * \author Sébastien Angibaud
 */
#ifndef __BF_COLOR_EDIT_HPP__
#define __BF_COLOR_EDIT_HPP__

#include "bf/base_edit.hpp"
#include "bf/default_value.hpp"
#include "bf/color.hpp"

#include "bf/spin_ctrl.hpp"

#include "bf/libeditor_export.hpp"

#include <wx/wx.h>
#include <wx/clrpicker.h>

namespace bf
{
  /**
   * \brief Control for editing a field whose value is a a color.
   * \author Sébastien Angibaud
   */
  class BEAR_EDITOR_EXPORT color_edit:
    public wxPanel,
    public base_edit<color>
  {
  public:
    color_edit
      ( wxWindow& parent, const color& v = default_value<color>::get() );
    color_edit
      ( wxWindow* parent, const color& v = default_value<color>::get() );

    bool validate();

  private:
    color make_color() const;

    void value_updated();
    void fill_controls();

    void create_controls();
    void create_sizer_controls();

  private:
    /** \brief The control for setting the color. */
    wxColourPickerCtrl * m_picker;

    /** \brief The control for setting the opacity of the color. */
    bf::spin_ctrl<double>* m_opacity_spin;
  }; // class color_edit
} // namespace bf

#endif // __BF_COLOR_EDIT_HPP__
