/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The window displaying the list of layers in the level.
 * \author Julien Jorge
 */
#ifndef __BF_LAYER_LIST_FRAME_HPP__
#define __BF_LAYER_LIST_FRAME_HPP__

#include "bf/item_filter/item_filter.hpp"

#include <wx/wx.h>

namespace bf
{
  class ingame_view;
  class level_action;

  /**
   * \brief The window displaying the list of layers in the level.
   * \author Julien Jorge
   */
  class layer_list_frame:
    public wxPanel
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_LAYER_LIST,
        IDC_FILTER_LIST,
        IDC_PRIORITY_LIST,
        IDC_FILTER_ENTRY,
        IDC_APPLY_TO_CURRENT_LAYER,
        IDC_APPLY_TO_ALL_LAYERS,
        IDC_DELETE_FILTER,
        IDC_DELETE_FILTER_EVERYWHERE,

        IDC_NEW_PRIORITY,
        IDC_DELETE_PRIORITY,
        IDC_DECREASE_PRIORITY,
        IDC_INCREASE_PRIORITY,

        IDC_STRETCH_LAYER
      }; // enum control_id

  public:
    layer_list_frame( wxWindow* parent );

    void set_level_view( ingame_view* view );

    void refresh();

  private:
    void create_controls();

    wxWindow* create_layer_list( wxWindow* parent );
    wxWindow* create_filters_page( wxWindow* parent );
    wxWindow* create_priority_page( wxWindow* parent );

    void clear();
    void fill();

    void fill_layer_list();
    void fill_filter_list();
    void fill_priority_list();

    item_filter create_filter_from_entry() const;

    void remove_selected_filters();

    level_action* create_stretch_layer_action
    ( unsigned int index, bool fit_level, unsigned int width,
      unsigned int height ) const;

    void delete_selected_layer();
    void copy_selected_layer() const;

    void show_item_position_error();

    void diffuse_tag_visibility( int index );

    void on_new_layer(wxCommandEvent& event);
    void on_delete(wxCommandEvent& event);

    void on_show_properties(wxCommandEvent& event);
    void on_stretch_layer(wxCommandEvent& event);
    void on_toggle_layer(wxCommandEvent& event);
    void on_select_layer(wxCommandEvent& event);

    void on_move_up(wxCommandEvent& event);
    void on_move_down(wxCommandEvent& event);

    void on_close(wxCloseEvent& event);

    void on_insert_filter(wxCommandEvent& event);
    void on_multiple_insert_filter(wxCommandEvent& event);

    void on_delete_filter(wxCommandEvent& event);
    void on_delete_filter_everywhere(wxCommandEvent& event);

    void on_new_priority(wxCommandEvent& event);
    void on_delete_priority(wxCommandEvent& event);
    void on_decrease_priority(wxCommandEvent& event);
    void on_increase_priority(wxCommandEvent& event);

    void on_cut( wxCommandEvent& event );
    void on_copy( wxCommandEvent& event );
    void on_paste( wxCommandEvent& event );

  private:
    /** \brief The level we are working on. */
    ingame_view* m_level_view;

    /** \brief The list of layers, presented to the user. */
    wxCheckListBox* m_layer_list;

    /** \brief The list of the items to compile first on the current layer. */
    wxListBox* m_priority_list;

    /** \brief The list of filters on the current layer. */
    wxListBox* m_filter_list;

    /** \brief The text entry where the user can enter a new filter. */
    wxTextCtrl* m_filter_entry;

    DECLARE_EVENT_TABLE()

  }; // class layer_list_frame
} // namespace bf

#endif // __BF_LAYER_LIST_FRAME_HPP__
