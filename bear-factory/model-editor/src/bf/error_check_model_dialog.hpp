/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A dialog to check errors in a model.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ERROR_CHECK_MODEL_DIALOG_HPP__
#define __BF_ERROR_CHECK_MODEL_DIALOG_HPP__

#include <wx/dialog.h>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <list>

namespace bf
{
  class action;

  /**
   * \brief A dialog when the model is not valid.
   * \author Sebastien Angibaud
   */
  class error_check_model_dialog:
    public wxDialog
  {
  public:
    error_check_model_dialog
    ( wxWindow& parent,
      const std::list< const action* >& actions );

    unsigned int get_index_selected() const;

  private:
    void create_controls();
    void create_sizers();
    void adjust_last_column_size();
    void on_size(wxSizeEvent& event);

  private:
    /** \brief The list of bad actions. */
    const std::list< const action* >& m_actions;

    /** \brief The control that displays the list of bad actions. */
    wxListView* m_actions_list;

    DECLARE_EVENT_TABLE()

  }; // class error_check_model_dialog
} // namespace bf

#endif // __BF_ERROR_CHECK_MODEL_DIALOG_HPP__
