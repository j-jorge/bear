/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An accordion of controls.
 * \author Julien Jorge
 */
#ifndef __BF_ACCORDION_HPP__
#define __BF_ACCORDION_HPP__

#include <wx/panel.h>
#include <wx/tglbtn.h>
#include <map>

namespace bf
{
  /**
   * \brief An accordion of controls.
   *
   * Each added control is associated with a button that can be used to show or
   * Hide the control. At most one control can be visible at once.
   *
   * \author Julien Jorge
   */
  class accordion:
    public wxPanel
  {
  public:
    accordion( wxWindow* parent, int orient );

    void add( wxWindow* c, const wxString& label );
    void add( wxSizer* s, const wxString& label );

  private:
    void on_button( wxCommandEvent& event );

  private:
    /** \brief The sizer in which the parts of the accordion are added. */
    wxSizer* m_content;

    /** \brief The button of the currently visible sizer. */
    wxToggleButton* m_current_button;

    /** \brief The sizers associated with each button. */
    std::map<wxToggleButton*, wxSizer*> m_sizers;

  }; // accordion
} // namespace bf

#endif // __BF_ACCORDION_HPP__
