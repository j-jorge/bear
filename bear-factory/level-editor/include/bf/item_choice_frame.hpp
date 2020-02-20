/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The window showing the item choice for selection.
 * \author Sebastien Angibaud
 */
#ifndef __BF_ITEM_CHOICE_FRAME_HPP__
#define __BF_ITEM_CHOICE_FRAME_HPP__

#include "bf/item_selection.hpp"

#include <wx/wx.h>
#include <set>

namespace bf
{
  class item_instance;

  /**
   * \brief The window showing the item choice for selection.
   * \author Sebastien Angibaud
   */
  class item_choice_frame:
    public wxDialog
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_LAYER_LIST,
        IDC_INVERSE,
        IDC_SELECT_ALL
      }; // enum control_id

  public:
    item_choice_frame
    ( wxWindow* parent, const item_selection& selected_group,
      const std::set<item_instance*>& choice);

    void get_selection(std::set<item_instance*>& selected_items);

  private:
    void create_controls();
    wxSizer* create_list_sizer();

    void select_all();
    void inverse_selection();

    void on_ok(wxCommandEvent& event);
    void on_inverse(wxCommandEvent& event);
    void on_select_all(wxCommandEvent& event);

  private:
    /* \brief The checklistbox of items. */
    wxCheckListBox* m_item_checklistbox;

    /* \brief The list of choice. */
    const std::set<item_instance*> m_choice;

    /* \brief The selected group. */
    const item_selection m_selected_group;

    /* \brief The button to inverse the selection. */
    wxButton* m_inverse_button;

    /* \brief The button to select all items. */
    wxButton* m_all_button;

    DECLARE_EVENT_TABLE()

  }; // class item_choice_frame
} // namespace bf

#endif // __BF_ITEM_CHOICE_FRAME_HPP__
