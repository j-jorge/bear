/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The window showing the properties of a mark.
 * \author S�bastien Angibaud
 */
#ifndef __BF_MARK_PROPERTIES_FRAME_HPP__
#define __BF_MARK_PROPERTIES_FRAME_HPP__

#include <wx/wx.h>
#include <wx/textctrl.h>

#include "bf/mark.hpp"

namespace bf
{
  class action;
  class any_animation_edit;
  class workspace_environment;

  /**
   * \brief The mark_properties window of our program.
   * \author S�bastien Angibaud
   */
  class mark_properties_frame:
    public wxDialog
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_EDIT_ANIMATION
      }; // enum control_id

  public:
    mark_properties_frame
    ( wxWindow* parent, const action* a, workspace_environment& env );

    const std::string& get_mark_name() const;
    const bf::any_animation& get_mark_animation() const;
    bool apply_angle_to_animation() const;
    bool pause_animation_when_hidden() const;
    bool reset_animation_with_action() const;

    void fill_from( const mark& a );

  private:
    void fill_controls();

    void create_controls();
    void create_member_controls();
    void create_sizer_controls();

    void on_ok(wxCommandEvent& event);
    void on_edit_animation(wxCommandEvent& event);

  private:
    /** \brief The current action. */
    const action* m_action;

    /** \brief The label of the mark. */
    std::string m_mark_label;

    /** \brief The animation of the mark. */
    bf::any_animation m_mark_animation;

    /** \brief Tell if the angle applied to the mark is also applied to the
        animation. */
    bool m_apply_angle_to_animation;

    /** \brief Tell if the animation is paused when the mark is hidden. */
    bool m_pause_animation_when_hidden;

    /** \brief Tells if the animation must be reset when an action begins. */
    bool m_reset_animation_with_action;

    /** \brief The text control in which we configure the label of the mark. */
    wxTextCtrl* m_mark_label_box;

    /** \brief The button to edit the animation. */
    wxButton* m_edit_animation;

    /** \brief The control to check the angle application statut. */
    wxCheckBox* m_apply_angle_box;

    /** \brief The control to check if the animation is paused when
        the mark is hidden. */
    wxCheckBox* m_pause_when_hidden_box;

    /** \brief The control to check if the animation is reset when the action
        containing the mark begins. */
    wxCheckBox* m_reset_with_action_box;

    /** \brief The workspace environment to use. */
    workspace_environment& m_workspace;

    DECLARE_EVENT_TABLE()

  }; // class mark_properties_frame
} // namespace bf

#endif // __BF_MARK_PROPERTIES_FRAME_HPP__
