/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The window showing the properties of the grid.
 * \author Sebastien Angibaud
 */
#ifndef __BF_GRID_PROPERTIES_FRAME_HPP__
#define __BF_GRID_PROPERTIES_FRAME_HPP__

#include "bf/grid.hpp"

#include <wx/wx.h>
#include <wx/spinctrl.h>

namespace bf
{
  /**
   * \brief The grid properties window of our program.
   * \author Sebastien Angibaud
   */
  class grid_properties_frame:
    public wxDialog
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_CHANGE_POSITION_SIZE
      }; // enum control_id

  public:
    grid_properties_frame( wxWindow* parent, const grid& g );

    const grid& get_grid() const;

  private:
    void fill_controls();

    void create_controls();
    void create_member_controls();
    void create_sizer_controls();

    void on_ok(wxCommandEvent& event);
    void on_value_change(wxSpinEvent& event);

  private:
    /** \brief The grid with the current values. */
    grid m_grid;

    /** \brief The text control in which we configure the width of the
        offset. */
    wxSpinCtrl* m_offset_x_text;

    /** \brief The text control in which we configure the height of the
        offset. */
    wxSpinCtrl* m_offset_y_text;

    /** \brief The text control in which we configure the width of the step. */
    wxSpinCtrl* m_step_x_text;

    /** \brief The text control in which we configure the height of the step. */
    wxSpinCtrl* m_step_y_text;

    /** \brief The control for configure the magnetism activation. */
    wxCheckBox* m_magnetism_active_box;

     /** \brief The control for configure
  the magnetism force. */
    wxSlider* m_magnetism_force_slider;

    /** \brief Minimum x_coordinate of step. */
    static const unsigned int s_min_step_x;

    /** \brief Minimum y_coordinate of step. */
    static const unsigned int s_min_step_y;

    DECLARE_EVENT_TABLE()

  }; // class grid_properties_frame
} // namespace bf

#endif // __BF_GRID_PROPERTIES_FRAME_HPP__
