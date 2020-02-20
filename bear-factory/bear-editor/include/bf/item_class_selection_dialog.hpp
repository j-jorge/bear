/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A dialog to select an item class.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_CLASS_SELECTION_DIALOG_HPP__
#define __BF_ITEM_CLASS_SELECTION_DIALOG_HPP__

#include <string>
#include <wx/dialog.h>
#include <wx/wx.h>

namespace bf
{
  class class_tree_ctrl;
  class class_selected_event;
  class workspace_environment;

  /**
   * \brief A dialog to select an item class.
   * \author Julien Jorge
   */
  class item_class_selection_dialog:
    public wxDialog
  {
  public:
    item_class_selection_dialog
    ( wxWindow* parent, workspace_environment& env,
      const std::string& class_name = std::string() );

    const std::string& get_class_name() const;

  private:
    void on_ok(wxCommandEvent& event);
    void on_class_selected( class_selected_event& event );

  private:
    /** \brief The name of the selected class. */
    std::string m_class_name;

    /** \brief The tree of classes. */
    class_tree_ctrl* m_tree;

    DECLARE_EVENT_TABLE()

  }; // class item_class_selection_dialog
} // namespace bf

#endif // __BF_ITEM_CLASS_SELECTION_DIALOG_HPP__
