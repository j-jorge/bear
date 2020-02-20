/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The dialog to configure the paths to the datas.
 * \author Julien Jorge
 */
#ifndef __BF_CONFIG_FRAME_HPP__
#define __BF_CONFIG_FRAME_HPP__

#include <wx/wx.h>
#include <map>
#include <string>
#include <list>

#include "bf/libeditor_export.hpp"
#include "bf/path_configuration.hpp"

namespace bf
{
  /**
   * \brief The dialog to configure the paths to the datas.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT config_frame:
    public wxDialog
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_BROWSE_ITEM_CLASSES_BUTTON,
        IDC_ERASE_ITEM_CLASSES_BUTTON,
        IDC_BROWSE_DATA_PATH_BUTTON,
        IDC_ERASE_DATA_PATH_BUTTON,
        IDC_BROWSE_RUN_PATH_BUTTON,
        IDC_CHOICE
      }; // enum control_id

  public:
    config_frame( wxWindow* parent );

  private:

    void fill_controls();
    void fill_list_view();

    void create_controls();
    void create_member_controls();
    void create_sizer_controls();

    void on_ok(wxCommandEvent& event);
    void on_cancel(wxCommandEvent& event);
    void on_browse_item_classes(wxCommandEvent& event);
    void on_erase_item_classes(wxCommandEvent& event);
    void on_browse_data_path(wxCommandEvent& event);
    void on_erase_data_path(wxCommandEvent& event);
    void on_select_workspace(wxCommandEvent& event);
    void on_browse_run_path(wxCommandEvent& event);

  private:
    /** \brief The list of workspaces. */
    wxChoice* m_workspaces_choice;

    /** \brief The list of paths to the item classes files. */
    wxListBox* m_item_classes_list;

    /** \brief The list of paths to the data of the game. */
    wxListBox* m_data_path_list; 

    /** \brief The path to the run configuration. */
    wxListBox* m_run_path;

    /** \brief Map of workspaces. */
    path_configuration::workspaces_map m_workspaces;

    DECLARE_EVENT_TABLE()

  }; // class config_frame
} // namespace bf

#endif // __BF_CONFIG_FRAME_HPP__
