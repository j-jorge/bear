/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A spin control that supports any numeric type of value.
 * \author Julien Jorge
 */
#ifndef __BF_SPIN_CTRL_HPP__
#define __BF_SPIN_CTRL_HPP__

#include <wx/spinbutt.h>
#include <wx/textctrl.h>
#include <wx/wx.h>

#define spin_event_handler(func, T)                                     \
  (wxObjectEventFunction)(wxEventFunction)                              \
    wxStaticCastEvent(spin_event<T>::function_type, &func)

#define EVT_SPIN_VALUE_CHANGE(id, func, T)                              \
  wx__DECLARE_EVT1( bf::spin_event<T>::value_change_event_type,         \
                    id, spin_event_handler(func, T) )

namespace bf
{
  /**
   * \brief Event sent when the value of a spin_ctrl has changed.
   * \author Julien Jorge
   */
  template<typename T>
  class spin_event:
    public wxNotifyEvent
  {
  public:
    typedef void (wxEvtHandler::*function_type)(spin_event<T>&);

  public:
    spin_event( wxEventType t = wxEVT_NULL, wxWindowID id = wxID_ANY );
    spin_event( T value, wxEventType t = wxEVT_NULL, wxWindowID id = wxID_ANY );
    spin_event( const spin_event<T>& that );

    wxEvent* Clone() const;

    T get_value() const;

  public:
    static const wxEventType value_change_event_type;

  private:
    /** \brief The value of the spin_ctrl. */
    const T m_value;

  }; // class spin_event

  /**
   * \brief A spin control that supports any numeric type of value.
   * \author Julien Jorge
   */
  template<typename T>
  class spin_ctrl:
    public wxPanel
  {
  public:
    /** \brief The type of the value stored in the control. */
    typedef T value_type;

    /** \brief The type of the parent class. */
    typedef wxPanel super;

    /** \brief The type of the current class. */
    typedef spin_ctrl<T> self_type;

  public:
    spin_ctrl
    ( wxWindow* parent, wxWindowID id = -1,
      const wxPoint& pos = wxDefaultPosition,
      const wxSize& size = wxDefaultSize,
      long style = 0, value_type min = 0, value_type max = 100,
      value_type initial = 0, value_type step = 1,
      const wxString& name = wxT("spin_ctrl") );

    void SetRange( value_type min, value_type max );
    void SetStep( value_type s );

    void SetValue( value_type v );
    value_type GetValue() const;
    value_type GetMin() const;
    value_type GetMax() const;
    wxString GetValueText() const;

  private:
    void BoundValue( value_type v );

    void ValueToText();
    void DoValueToText();

    void CreateControls();

    void Up();
    void Down();
    void SendEvent();

  protected:
    void OnSpinUp( wxSpinEvent& event );
    void OnSpinDown( wxSpinEvent& event );
    void OnChange( wxCommandEvent& event );

  private:
    /** \brief The minimum value. */
    value_type m_min;

    /** \brief The maximum value. */
    value_type m_max;

    /** \brief The current value. */
    value_type m_value;

    /** \brief The step of the spin button. */
    value_type m_step;

    /** \brief The text field in which we display the value. */
    wxTextCtrl* m_text;

    /** \brief The spin button. */
    wxSpinButton* m_spin;

  }; // spin_ctrl
} // namespace bf

#include "bf/impl/spin_ctrl.tpp"

#endif // __BF_SPIN_CTRL_HPP__
