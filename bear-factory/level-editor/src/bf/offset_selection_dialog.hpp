/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A dialog to apply an offset the selection.
 * \author Julien Jorge
 */
#ifndef __BF_OFFSET_SELECTION_DIALOG_HPP__
#define __BF_OFFSET_SELECTION_DIALOG_HPP__

#include "bf/spin_ctrl.hpp"

#include <wx/dialog.h>

namespace bf
{
  /**
   * \brief A dialog to apply offset the selection.
   * \author Julien Jorge
   */
  class offset_selection_dialog:
    public wxDialog
  {
  public:
    offset_selection_dialog( wxWindow& parent );

    double get_x() const;
    double get_y() const;

  private:
    void create_controls();
    void create_sizers();

  private:
    /** \brief Offset on the x-axis. */
    spin_ctrl<double>* m_x;

    /** \brief Offset on the y-axis. */
    spin_ctrl<double>* m_y;

  }; // class offset_selection_dialog
} // namespace bf

#endif // __BF_OFFSET_SELECTION_DIALOG_HPP__
