/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The window showing the date of a snapshot.
 * \author Sébastien Angibaud
 */
#ifndef __BF_SNAPSHOT_DATE_FRAME_HPP__
#define __BF_SNAPSHOT_DATE_FRAME_HPP__

#include <wx/wx.h>
#include <wx/textctrl.h>

#include "bf/spin_ctrl.hpp"

namespace bf
{
  class action;
  class snapshot;

  /**
   * \brief The snapshot_date window of our program.
   * \author Sébastien Angibaud
   */
  class snapshot_date_frame:
    public wxDialog
  {
  public:
    snapshot_date_frame( wxWindow* parent, const action* a );

    double get_date() const;

    void fill_from( const snapshot* a );

  private:
    void fill_controls();

    void create_controls();
    void create_member_controls();
    void create_sizer_controls();

    void on_ok(wxCommandEvent& event);

  private:
    /** \brief The current action. */
    const action* m_action;

    /** \brief The date of the snapshot. */
    double m_date;

    /** \brief The spin control in which we configure the date of
        the snapshot. */
    spin_ctrl<double>* m_date_spin;

    DECLARE_EVENT_TABLE()

  }; // class snapshot_date_frame
} // namespace bf

#endif // __BF_SNAPSHOT_DATE_FRAME_HPP__
