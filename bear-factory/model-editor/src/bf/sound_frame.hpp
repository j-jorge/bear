/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A component to edit a sound_description.
 * \author Julien Jorge
 */
#ifndef __BF_SOUND_FRAME_HPP__
#define __BF_SOUND_FRAME_HPP__

#include "bf/sound_description.hpp"

#include <wx/wx.h>
#include <string>

#define sound_description_event_handler(func)                           \
  (wxObjectEventFunction)(wxEventFunction)                              \
  wxStaticCastEvent(bf::sound_description_event::function_type, &func)

#define EVT_SOUND_DESCRIPTION_CHANGE(id, func)                         \
  wx__DECLARE_EVT1( bf::sound_description_event::change_event_type,    \
                    id, sound_description_event_handler(func) )

namespace bf
{
  class workspace_environment;

  /**
   * \brief Event sent when something has changed in the sound frame.
   * \author Julien Jorge
   */
  class sound_description_event:
    public wxNotifyEvent
  {
  public:
    typedef void (wxEvtHandler::*function_type)(sound_description_event&);

  public:
    sound_description_event
    ( wxEventType t = wxEVT_NULL, wxWindowID id = wxID_ANY );

    wxEvent* Clone() const;

  public:
    static const wxEventType change_event_type;

  }; // class sound_description_event

  /**
   * \brief A component to edit a sound_description.
   * \author Julien Jorge
   */
  class sound_frame:
    public wxPanel
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        ID_PLAY_GLOBALLY
      }; // enum control_id

  public:
    sound_frame
    ( wxWindow* parent, wxWindowID id = wxID_ANY, workspace_environment* env );

    void set_sound_description( sound_description d );
    sound_description get_sound_description() const;

  private:
    void create_controls();
    void create_member_controls();
    void create_sizer_controls();

    void dispatch_change_event();

    void on_new_sound( wxCommandEvent& event );
    void on_edit_sound( wxCommandEvent& event );
    void on_remove_sound( wxCommandEvent& event );

    void on_play_globally_change( wxCommandEvent& event );

  private:
    /** \brief The list of the sound files. */
    wxListBox* m_sound_files;

    /** \brief Tells if the sound is played globally. */
    wxCheckBox* m_play_globally;

    /** \brief The workspace environment used. */
    workspace_environment* m_workspace; 
    
    DECLARE_EVENT_TABLE()

  }; // class sound_frame
} // namespace bf

#endif // __BF_SOUND_FRAME_HPP__
