/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/*
 * \file
 * \brief Implementation of the bf::item_event class.
 * \author Julien Jorge
 */
#include "bf/item_event.hpp"

/*----------------------------------------------------------------------------*/
const wxEventType bf::item_event::selection_event_type = wxNewEventType();

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param t The type of the event.
 * \param id the id of the window that generates the event.
 */
bf::item_event::item_event( wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id), m_item(NULL)
{

} // item_event::item_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item concerned by this event.
 * \param t The type of the event.
 * \param id the id of the window that generates the event.
 */
bf::item_event::item_event( item_instance* item, wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id), m_item(item)
{

} // item_event::item_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bf::item_event::item_event( const item_event& that )
  : wxNotifyEvent(that), m_item(that.m_item)
{

} // item_event::item_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
wxEvent* bf::item_event::Clone() const
{
  return new item_event(*this);
} // item_event::Clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the item concerned by this event.
 */
bf::item_instance* bf::item_event::get_item() const
{
  return m_item;
} // item_event::get_item()
