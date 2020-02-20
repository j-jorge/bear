/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The window showing the properties of a bitmap_rendering_attributes.
 * \author Julien Jorge
 */
#ifndef __BF_BITMAP_RENDERING_ATTRIBUTES_EDIT_HPP__
#define __BF_BITMAP_RENDERING_ATTRIBUTES_EDIT_HPP__

#include "bf/base_edit.hpp"
#include "bf/bitmap_rendering_attributes.hpp"
#include "bf/color_edit.hpp"
#include "bf/spin_ctrl.hpp"
#include "bf/libeditor_export.hpp"

#include <wx/wx.h>
#include <wx/spinctrl.h>

namespace bf
{
  /**
   * \brief The window showing the properties of a bitmap_rendering_attributes.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT bitmap_rendering_attributes_edit:
    public wxPanel,
    public base_edit<bitmap_rendering_attributes>
  {
  private:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        ID_AUTO_SIZE
      }; // enum control_id

  public:
    bitmap_rendering_attributes_edit
    ( wxWindow& parent, const bitmap_rendering_attributes& spr );

    bool validate();

  private:
    void value_updated();
    void fill_controls();

    void create_controls();
    wxSizer* create_size_and_placement_sizer();
    wxSizer* create_color_sizer();

    void on_auto_size( wxCommandEvent& event );

  private:
    /** \brief Tell if the size is automatically computed. */
    wxCheckBox* m_auto_size;

    /** \brief The text control in which we configure the width. */
    wxSpinCtrl* m_width_spin;

    /** \brief The text control in which we configure the height. */
    wxSpinCtrl* m_height_spin;

    /** \brief The control for configuring the angle. */
    bf::spin_ctrl<double>* m_angle_spin;

    /** \brief The control for configuring the flip_x option. */
    wxCheckBox* m_flip_x_box;

    /** \brief The control for configuring the flip_y option. */
    wxCheckBox* m_flip_y_box;

    /** \brief The control for configuring the color. */
    color_edit* m_color_ctrl;

    DECLARE_EVENT_TABLE()

  }; // class bitmap_rendering_attributes_edit
} // namespace bf

#endif // __BF_BITMAP_RENDERING_ATTRIBUTES_EDIT_HPP__
