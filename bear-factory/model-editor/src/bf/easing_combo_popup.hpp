/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A combobox containing all the easing functions.
 * \author Julien Jorge
 */
#ifndef __BF_EASING_COMBO_POPUP_HPP__
#define __BF_EASING_COMBO_POPUP_HPP__

#include <wx/combo.h>
#include <wx/listctrl.h>

namespace bf
{
  /**
   * \brief A combobox containing all the easing functions.
   * \author Julien Jorge
   */
  class easing_combo_popup:
    public wxListView,
    public wxComboPopup
  {
  public:
    virtual bool Create(wxWindow* parent);
    virtual wxWindow* GetControl();

    virtual void SetStringValue( const wxString& s );
    virtual wxString GetStringValue() const;

  private:
    void connect_list_events();
    void disconnect_list_events();

    void append_item( const wxString& label, const wxBitmap& img );
    void fill_list();

    void on_item_activated( wxListEvent& event );

  private:

    DECLARE_EVENT_TABLE()

  }; // class easing_combo_popup
} // namespace bf

#endif // __BF_EASING_COMBO_POPUP_HPP__
