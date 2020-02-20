/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A dialog to clone the selection.
 * \author Julien Jorge
 */
#ifndef __BF_CLONE_SELECTION_DIALOG_HPP__
#define __BF_CLONE_SELECTION_DIALOG_HPP__

#include "bf/spin_ctrl.hpp"

#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>

namespace bf
{
  /**
   * \brief A dialog to clone the selection.
   * \author Julien Jorge
   */
  class clone_selection_dialog:
    public wxDialog
  {
  public:
    clone_selection_dialog( wxWindow& parent );

    unsigned int get_x_count() const;
    unsigned int get_y_count() const;
    double get_x_offset() const;
    double get_y_offset() const;
    bool get_add_to_selection() const;

  private:
    void create_controls();
    void create_sizers();

  private:
    /** \brief Count on the x-axis. */
    wxSpinCtrl* m_x_count;

    /** \brief Count on the y-axis. */
    wxSpinCtrl* m_y_count;

    /** \brief Offset on the x-axis. */
    spin_ctrl<double>* m_x_offset;

    /** \brief Offset on the y-axis. */
    spin_ctrl<double>* m_y_offset;

    /** \brief Add the clones to the selection. */
    wxCheckBox* m_add_to_selection;

  }; // class clone_selection_dialog
} // namespace bf

#endif // __BF_CLONE_SELECTION_DIALOG_HPP__
