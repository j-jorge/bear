/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The accelerator table stores the accelerators and their command for a
 *        given event handler.
 * \author Julien Jorge
 */
#ifndef __BF_ACCELERATOR_TABLE__
#define __BF_ACCELERATOR_TABLE__

#include <map>
#include <wx/window.h>

#include "bf/libeditor_export.hpp"

namespace bf
{
  /**
   * \brief The accelerator table stores the accelerators and their command for
   *        a given event handler.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT accelerator_table:
    public wxEvtHandler
  {
  public:
    /**
     * \brief The state of the control key in an accelerator.
     */
    enum control_state
    {
      control_released,
      control_pressed
    }; // enum control_state

    /**
     * \brief The state of the alt key in an accelerator.
     */
    enum alt_state
    {
      alt_released,
      alt_pressed
    }; // enum alt_state

    /**
     * \brief The state of the shift key in an accelerator.
     */
    enum shift_state
    {
      shift_released,
      shift_pressed
    }; // enum shift_state

  private:
    /**
     * \brief The description of the keyboard state for an accelerator.
     */
    class entry
    {
    public:
      entry
      ( int key_code, control_state ctrl, alt_state alt, shift_state shift );

      bool operator<( const entry& that ) const;

    private:
      /** \brief The code of the key to press to trigger the accelerator. */
      const int m_key_code;

      /** \brief The state of the control key to trigger the accelerator. */
      const control_state m_control_state;

      /** \brief The state of the alt key to trigger the accelerator. */
      const alt_state m_alt_state;

      /** \brief The state of the shift key to trigger the accelerator. */
      const shift_state m_shift_state;

    }; // class entry

    /** \brief The map associating the commands to the accelerators. */
    typedef std::map<entry, int> entry_map;

  public:
    accelerator_table( wxWindow& event_handler );

    void add_accelerator
      ( wxEventType event_type, int key_code,
        control_state ctrl = control_released, alt_state alt = alt_released,
        shift_state shift = shift_released );

  private:
    void on_key_pressed( wxKeyEvent& event );

  private:
    /** \brief The instance that dispatches the key events on on which we
        dispatch the command events. */
    wxWindow& m_event_handler;

    /** \brief The accelerators. */
    entry_map m_accelerators;

  }; // class accelerator_table
} // namespace bf

#endif // __BF_ACCELERATOR_TABLE__
