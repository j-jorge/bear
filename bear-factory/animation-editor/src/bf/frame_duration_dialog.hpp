/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A dialog shown to change the duration of all frames at once.
 * \author Julien Jorge
 */
#ifndef __BF_FRAME_DURATION_DIALOG_HPP__
#define __BF_FRAME_DURATION_DIALOG_HPP__

#include "bf/spin_ctrl.hpp"

namespace bf
{
  /**
   * \brief A dialog shown to change the duration of all frames at once.
   * \author Julien Jorge
   */
  class frame_duration_dialog:
    public wxDialog
  {
  public:
    /** \brief How the value must be applied to the frames. */
    enum duration_application_mode
      {
        duration_replace,
        duration_add,
        duration_multiply
      }; // enum duration_application_mode

  public:
    frame_duration_dialog( wxWindow* parent );

    double get_operand() const;
    duration_application_mode get_mode() const;

  private:
    void create_controls();

  private:
    /** \brief How to apply the operand to the durations. */
    wxRadioBox* m_mode;

    /** \brief The operand applied to the frames. */
    spin_ctrl<double>* m_operand;

  }; // class frame_duration_dialog
} // namespace bf

#endif // __BF_FRAME_DURATION_DIALOG_HPP__
