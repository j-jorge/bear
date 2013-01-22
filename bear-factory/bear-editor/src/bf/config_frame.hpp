/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The dialog to configure the paths to the datas.
 * \author Julien Jorge
 */
#ifndef __BF_CONFIG_FRAME_HPP__
#define __BF_CONFIG_FRAME_HPP__

#include <wx/wx.h>

#include "bf/libeditor_export.hpp"

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
        IDC_ERASE_DATA_PATH_BUTTON
      }; // enum control_id

  public:
    config_frame( wxWindow* parent );

  private:

    void fill_controls();

    void create_controls();
    void create_member_controls();
    void create_sizer_controls();

    void on_ok(wxCommandEvent& event);
    void on_cancel(wxCommandEvent& event);
    void on_browse_item_classes(wxCommandEvent& event);
    void on_erase_item_classes(wxCommandEvent& event);
    void on_browse_data_path(wxCommandEvent& event);
    void on_erase_data_path(wxCommandEvent& event);

  private:
    /** \brief The list of paths to the item classes files. */
    wxListBox* m_item_classes_list;

    /** \brief The list of paths to the data of the game. */
    wxListBox* m_data_path_list;

    DECLARE_EVENT_TABLE()

  }; // class config_frame
} // namespace bf

#endif // __BF_CONFIG_FRAME_HPP__
