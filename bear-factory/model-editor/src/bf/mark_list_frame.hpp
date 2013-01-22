/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The window displaying the list of marks of an action.
 * \author Sébastien Angibaud
 */
#ifndef __BF_MARK_LIST_FRAME_HPP__
#define __BF_MARK_LIST_FRAME_HPP__

#include <wx/wx.h>
#include <string>

namespace bf
{
  class ingame_view;
  class model_frame;

  /**
   * \brief The window displaying the list of marks of an action.
   * \author Sébastien Angibaud
   */
  class mark_list_frame:
    public wxPanel
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_MARK_LIST
      }; // enum control_id

  public:
    mark_list_frame( wxWindow* parent);

    void set_model_frame( model_frame* f );
    void update_action();
    void update_mark();
  private:
    void select_mark( const std::string& mark_name );

  private:
    void create_controls();
    void create_member_controls();
    void create_sizer_controls();
    void update_controls();

    void clear();
    void fill();

    void on_new_mark(wxCommandEvent& event);
    void on_delete(wxCommandEvent& event);

    void on_show_properties(wxCommandEvent& event);
    void on_select_mark(wxCommandEvent& event);
    void on_close(wxCloseEvent& event);

  private:
    /** \brief The current model_frame. */
    model_frame* m_model_frame;

    /** \brief The list of marks, presented to the user. */
    wxListBox* m_mark_list;

    /** \brief The button to create a new mark. */
    wxButton* m_new_mark_button;

    /** \brief The button to delete a mark. */
    wxButton* m_delete_mark_button;

    /** \brief The button to edit the properties of a mark. */
    wxButton* m_properties_mark_button;

    DECLARE_EVENT_TABLE()

  }; // class mark_list_frame
} // namespace bf

#endif // __BF_MARK_LIST_FRAME_HPP__
