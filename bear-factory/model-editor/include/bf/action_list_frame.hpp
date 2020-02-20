/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The window displaying the list of actions in the model.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_LIST_FRAME_HPP__
#define __BF_ACTION_LIST_FRAME_HPP__

#include <wx/wx.h>
#include <string>

namespace bf
{
  class ingame_view;
  class model_frame;

  /**
   * \brief The window displaying the list of actions in the model.
   * \author Sébastien Angibaud
   */
  class action_list_frame:
    public wxPanel
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_ACTION_LIST
      }; // enum control_id

  public:
    action_list_frame( wxWindow* parent );

    void set_model_frame( model_frame* f );
    void select_action( const std::string& action_name );
    void update_action();

  private:
    void create_controls();
    void create_member_controls();
    void create_sizer_controls();
    void update_controls();

    void clear();
    void fill();

    void on_new_action(wxCommandEvent& event);
    void on_delete(wxCommandEvent& event);
    void on_copy(wxCommandEvent& event);

    void on_show_properties(wxCommandEvent& event);
    void on_select_action(wxCommandEvent& event);

    void on_close(wxCloseEvent& event);

  private:
    /** \brief The model we are working on. */
    model_frame* m_model_frame;

    /** \brief The list of actions, presented to the user. */
    wxListBox* m_action_list;

    /** \brief The button to create a new action. */
    wxButton* m_new_action_button;

    /** \brief The button to delete an action. */
    wxButton* m_delete_action_button;

    /** \brief The button to copy an action. */
    wxButton* m_copy_action_button;

    /** \brief The button to edit properties of an action. */
    wxButton* m_properties_action_button;

    DECLARE_EVENT_TABLE()

  }; // class action_list_frame
} // namespace bf

#endif // __BF_ACTION_LIST_FRAME_HPP__
