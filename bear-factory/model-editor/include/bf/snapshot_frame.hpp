/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The window displaying the current snapshot of an action.
 * \author Sébastien Angibaud
 */
#ifndef __BF_SNAPSHOT_FRAME_HPP__
#define __BF_SNAPSHOT_FRAME_HPP__

#include "bf/accordion.hpp"
#include "bf/base_file_edit.hpp"
#include "bf/sound_frame.hpp"
#include "bf/spin_ctrl.hpp"

#include <wx/combo.h>
#include <wx/spinctrl.h>
#include <wx/wx.h>
#include <string>

namespace bf
{
  class model_frame;
  class snapshot;

  /**
   * \brief The window displaying various informations about a snapshot.
   * \author Sébastien Angibaud
   */
  class snapshot_frame:
    public wxPanel
  {
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_DATE_CTRL,
        IDC_MARK_CHOICE,
        IDC_MARK_X,
        IDC_MARK_Y,
        IDC_MARK_ANGLE,
        IDC_MARK_DEPTH,
        IDC_MARK_WIDTH,
        IDC_MARK_HEIGHT,
        IDC_MARK_FUNCTION,
        IDC_VISIBILITY,
        IDC_FUNCTION,
        IDC_SOUND,
        IDC_ALIGNMENT,
        IDC_SIZE,
        IDC_ACTION_CHOICE
      }; // enum control_id

  public:
    snapshot_frame( wxWindow* parent);

    void set_model_frame( model_frame* f );
    void update_action();
    void update_snapshot();
    void update_mark();

  private:
    void create_controls();
    void create_member_controls( accordion* a );
    void create_sizer_controls( accordion* a );

    void create_snapshot_controls( wxWindow* parent );
    void create_marks_controls( wxWindow* parent );
    void create_sound_controls( wxWindow* parent );
    void create_item_box_controls( wxWindow* parent );

    wxSizer* create_action_sizer( wxWindow* parent );
    wxSizer* create_snapshot_sizer( wxWindow* parent );
    wxSizer* create_marks_sizer( wxWindow* parent );
    wxSizer* create_sound_sizer( wxWindow* parent );
    wxSizer* create_item_box_sizer( wxWindow* parent );

    wxComboCtrl* create_easing_combo( wxWindow* parent );

    void fill();
    void fill_mark();
    void fill_action();

    void alignment_change();
    void size_change();
    void sound_change();

    void enable_controls(bool enable);

    void on_close(wxCloseEvent& event);
    void on_date_change( wxCommandEvent& event );
    void on_mark_change( wxCommandEvent& event );
    void on_action_change( wxCommandEvent& event );
    void on_alignment_choice_change( wxCommandEvent& event );

    void on_mark_x( spin_event<double>& event );
    void on_mark_y( spin_event<double>& event );
    void on_mark_angle( spin_event<double>& event );
    void on_mark_width( spin_event<double>& event );
    void on_mark_height( spin_event<double>& event );

    void on_mark_function( wxCommandEvent& event );
    void on_alignment_change( spin_event<double>& event );
    void on_size_change( spin_event<double>& event );
    void on_depth_change( wxSpinEvent& event );
    void on_visibility_change( wxCommandEvent& event );
    void on_function_change( wxCommandEvent& event );
    void on_sound_change( sound_description_event& event );

    void on_x_easing( wxCommandEvent& event );
    void on_y_easing( wxCommandEvent& event );
    void on_width_easing( wxCommandEvent& event );
    void on_height_easing( wxCommandEvent& event );
    void on_angle_easing( wxCommandEvent& event );

  private:
    /** \brief The current model_frame. */
    model_frame* m_model_frame;

    /** \brief The list of marks, presented to the user. */
    snapshot* m_snapshot;

    /** \brief The button control in which we configure the date of
        the snapshot. */
    wxButton* m_date_button;

    /** \brief The text control in which we configure the function to call. */
    wxButton* m_function_button;

    /** \brief The spin control in which we configure the left of the mark.*/
    spin_ctrl<double>* m_mark_left_spin;

    /** \brief The spin control in which we configure the bottom of the mark.*/
    spin_ctrl<double>* m_mark_bottom_spin;

    /** \brief The spin control in which we configure the width of a mark.*/
    spin_ctrl<double>* m_mark_width_spin;

    /** \brief The spin control in which we configure the height of the mark.*/
    spin_ctrl<double>* m_mark_height_spin;

    /** \brief The text control in which we configure the function to call on
        a collision on a mark. */
    wxButton* m_collision_function_button;

    /** \brief The spin control in which we configure the z-coordinate of
        the box. */
    wxSpinCtrl* m_mark_depth_spin;

    /** \brief The spin control in which we configure the angle of the mark.*/
    spin_ctrl<double>* m_mark_angle_spin;

    /** \brief The checkbox that tells if the changes are done on the whole
        action. */
    wxCheckBox* m_modify_whole_action;

    /** \brief The spin control in which we configure the current action. */
    wxChoice* m_action_choice;

    /** \brief The spin control in which we configure the current mark. */
    wxChoice* m_mark_choice;

    /** \brief The button control to check the visibily of the mark. */
    wxCheckBox* m_mark_visible_box;

    /** \brief The easing function for the x-position of the mark. */
    wxComboCtrl* m_mark_x_easing;

    /** \brief The easing function for the y-position of the mark. */
    wxComboCtrl* m_mark_y_easing;

    /** \brief The easing function for the width of the mark. */
    wxComboCtrl* m_mark_width_easing;

    /** \brief The easing function for the height of the mark. */
    wxComboCtrl* m_mark_height_easing;

    /** \brief The easing function for the angle of the mark. */
    wxComboCtrl* m_mark_angle_easing;

    /** \brief Text control in which we display the path to the sound. */
    sound_frame* m_sound_frame;

    /** \brief The spin control in which we configure the
        width of the action.*/
    spin_ctrl<double>* m_width_spin;

    /** \brief The spin control in which we configure
        the height of the action.*/
    spin_ctrl<double>* m_height_spin;

    /** \brief The spin control in which we configure
        the value of alignment on x-coordinate.*/
    spin_ctrl<double>* m_x_alignment_value_spin;

    /** \brief The spin control in which we configure
        the value of alignment on y-coordinate.*/
    spin_ctrl<double>* m_y_alignment_value_spin;

    /** \brief The spin control in which we choose
        the type of alignment on x-coordinate.*/
    wxChoice* m_x_alignment_choice;

    /** \brief The spin control in which we choose
        the type of alignment on y-coordinate.*/
    wxChoice* m_y_alignment_choice;

    DECLARE_EVENT_TABLE()

  }; // class snapshot_frame
} // namespace bf

#endif // __BF_SNAPSHOT_FRAME_HPP__
