/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/*
 * \file
 * \brief An event sent when something happens to an item.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_EVENT_HPP__
#define __BF_ITEM_EVENT_HPP__

#include <wx/event.h>

#define item_event_handler(func)                                       \
  (wxObjectEventFunction)(wxEventFunction)                             \
    wxStaticCastEvent(item_event::function_type, &func)

#define EVT_ITEM_SELECTION(id, func)                          \
  wx__DECLARE_EVT1( bf::item_event::selection_event_type,     \
                    id, item_event_handler(func) )

namespace bf
{
  class item_instance;

  /**
   * \brief An event sent when something happens to an item.
   * \author Julien Jorge
   */
  class item_event:
    public wxNotifyEvent
  {
  public:
    typedef void (wxEvtHandler::*function_type)(item_event&);

  public:
    explicit item_event( wxEventType t = wxEVT_NULL, wxWindowID id = wxID_ANY );
    explicit item_event
    ( item_instance* item, wxEventType t = wxEVT_NULL,
      wxWindowID id = wxID_ANY );
    item_event( const item_event& that );

    wxEvent* Clone() const;

    item_instance* get_item() const;

  public:
    static const wxEventType selection_event_type;

  private:
    /** The item concerned by the event. */
    item_instance* const m_item;

  }; // class item_event

} // namespace bf

#endif // __BF_ITEM_EVENT_HPP__
