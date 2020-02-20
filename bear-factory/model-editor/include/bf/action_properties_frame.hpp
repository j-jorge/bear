/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The window showing the properties of a action.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_PROPERTIES_FRAME_HPP__
#define __BF_ACTION_PROPERTIES_FRAME_HPP__

#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include "bf/action.hpp"
#include "bf/spin_ctrl.hpp"
#include "bf/base_file_edit.hpp"

namespace bf
{
  class gui_model;
  class sound_frame;

  /**
   * \brief The action_properties window of our program.
   * \author Sébastien Angibaud
   */
  class action_properties_frame:
    public wxDialog
  {
  public:
    enum control_id
      {
        ID_DURATION_SPIN,
        ID_ALIGNMENT
      }; // enum control_id

  public:
    action_properties_frame( wxWindow* parent, const gui_model* m );

    double get_action_duration() const;
    const std::string& get_action_name() const;
    const sound_description& get_sound() const;
    const std::string& get_auto_next() const;

    void fill_from( const action& a );

  private:
    void fill_controls();

    void create_controls();
    void create_member_controls();
    void create_sizer_controls();

    void on_ok(wxCommandEvent& event);

  private:
    /** \brief The current model. */
    const gui_model* m_model;

    /** \brief The duration of the action. */
    double m_duration;

    /** \brief The name of the action. */
    std::string m_action_name;

    /** \brief The name of the action to start when this one is finished. */
    std::string m_auto_next;

    /** \brief The sound to play during the action. */
    sound_description m_sound;

    /** \brief The text control in which we configure the duration of the
        action. */
    spin_ctrl<double>* m_duration_spin;

    /** \brief The text control in which we configure the name of
        the action. */
    wxTextCtrl* m_action_name_box;

    /** \brief The control in which we configure the sound files. */
    sound_frame* m_sound_frame;

    /** \brief The text control in which we configure the next action. */
    wxTextCtrl* m_auto_next_text;

    DECLARE_EVENT_TABLE()

  }; // class action_properties_frame
} // namespace bf

#endif // __BF_ACTION_PROPERTIES_FRAME_HPP__
