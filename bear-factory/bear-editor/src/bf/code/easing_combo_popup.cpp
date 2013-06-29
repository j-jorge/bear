/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::easing_combo_popup.
 * \author Julien Jorge
 */
#include "bf/easing_combo_popup.hpp"

#include <wx/imaglist.h>

#include "bf/icon/easing/back_in_out.xpm"
#include "bf/icon/easing/back_in.xpm"
#include "bf/icon/easing/back_out.xpm"
#include "bf/icon/easing/bounce_in_out.xpm"
#include "bf/icon/easing/bounce_in.xpm"
#include "bf/icon/easing/bounce_out.xpm"
#include "bf/icon/easing/circ_in_out.xpm"
#include "bf/icon/easing/circ_in.xpm"
#include "bf/icon/easing/circ_out.xpm"
#include "bf/icon/easing/cubic_in_out.xpm"
#include "bf/icon/easing/cubic_in.xpm"
#include "bf/icon/easing/cubic_out.xpm"
#include "bf/icon/easing/elastic_in_out.xpm"
#include "bf/icon/easing/elastic_in.xpm"
#include "bf/icon/easing/elastic_out.xpm"
#include "bf/icon/easing/expo_in_out.xpm"
#include "bf/icon/easing/expo_in.xpm"
#include "bf/icon/easing/expo_out.xpm"
#include "bf/icon/easing/linear_in_out.xpm"
#include "bf/icon/easing/linear_in.xpm"
#include "bf/icon/easing/linear_out.xpm"
#include "bf/icon/easing/none_in_out.xpm"
#include "bf/icon/easing/none_in.xpm"
#include "bf/icon/easing/none_out.xpm"
#include "bf/icon/easing/quad_in_out.xpm"
#include "bf/icon/easing/quad_in.xpm"
#include "bf/icon/easing/quad_out.xpm"
#include "bf/icon/easing/quart_in_out.xpm"
#include "bf/icon/easing/quart_in.xpm"
#include "bf/icon/easing/quart_out.xpm"
#include "bf/icon/easing/quint_in_out.xpm"
#include "bf/icon/easing/quint_in.xpm"
#include "bf/icon/easing/quint_out.xpm"
#include "bf/icon/easing/sine_in_out.xpm"
#include "bf/icon/easing/sine_in.xpm"
#include "bf/icon/easing/sine_out.xpm"

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the popup control.
 * \param parent The parent control.
 */
bool bf::easing_combo_popup::Create(wxWindow* parent)
{
  if ( wxListView::Create
       ( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
         wxLC_ICON | wxLC_SINGLE_SEL ) )
    {
      fill_list();
      connect_list_events();
      return true;
    }
  else
    return false;
} // easing_combo_popup::Create()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the created control.
 */
wxWindow* bf::easing_combo_popup::GetControl()
{
  return this;
} // easing_combo_popup::GetControl()

/*----------------------------------------------------------------------------*/
/**
 * \brief Translate a string into a list selection.
 * \param s The string.
 */
void bf::easing_combo_popup::SetStringValue( const wxString& s )
{
  disconnect_list_events();
  wxListView::Select(wxListView::FindItem(-1, s));
  connect_list_events();
} // easing_combo_popup::SetStringValue()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the selection as a string.
 */
wxString bf::easing_combo_popup::GetStringValue() const
{
  return wxListView::GetItemText(wxListView::GetFirstSelected());
} // easing_combo_popup::GetStringValue()

/*----------------------------------------------------------------------------*/
/**
 * \brief Connect the event listener on the list.
 */
void bf::easing_combo_popup::connect_list_events()
{
  wxListView::Connect
    ( wxEVT_COMMAND_LIST_ITEM_SELECTED,
      wxListEventHandler(easing_combo_popup::on_item_activated) );
} // easing_combo_popup::connect_list_events()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disconnect the event listener on the list.
 */
void bf::easing_combo_popup::disconnect_list_events()
{
  wxListView::Disconnect
    ( wxEVT_COMMAND_LIST_ITEM_SELECTED,
      wxListEventHandler(easing_combo_popup::on_item_activated) );
} // easing_combo_popup::disconnect_list_events()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item in the list.
 * \param label The label of the item.
 * \param img The image of the item.
 */
void bf::easing_combo_popup::append_item
( const wxString& label, const wxBitmap& img )
{
  wxImageList* images = GetImageList( wxIMAGE_LIST_NORMAL );

  images->Add( img );
  InsertItem( GetItemCount(), label, images->GetImageCount()-1 );
} // easing_combo_popup::append_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the items in the list.
 */
void bf::easing_combo_popup::fill_list()
{
  AssignImageList( new wxImageList(), wxIMAGE_LIST_NORMAL );

  append_item( _("back:in_out"), wxBitmap(back_in_out_xpm) );
  append_item( _("back:in"), wxBitmap(back_in_xpm) );
  append_item( _("back:out"), wxBitmap(back_out_xpm) );
  append_item( _("bounce:in_out"), wxBitmap(bounce_in_out_xpm) );
  append_item( _("bounce:in"), wxBitmap(bounce_in_xpm) );
  append_item( _("bounce:out"), wxBitmap(bounce_out_xpm) );
  append_item( _("circ:in_out"), wxBitmap(circ_in_out_xpm) );
  append_item( _("circ:in"), wxBitmap(circ_in_xpm) );
  append_item( _("circ:out"), wxBitmap(circ_out_xpm) );
  append_item( _("cubic:in_out"), wxBitmap(cubic_in_out_xpm) );
  append_item( _("cubic:in"), wxBitmap(cubic_in_xpm) );
  append_item( _("cubic:out"), wxBitmap(cubic_out_xpm) );
  append_item( _("elastic:in_out"), wxBitmap(elastic_in_out_xpm) );
  append_item( _("elastic:in"), wxBitmap(elastic_in_xpm) );
  append_item( _("elastic:out"), wxBitmap(elastic_out_xpm) );
  append_item( _("expo:in_out"), wxBitmap(expo_in_out_xpm) );
  append_item( _("expo:in"), wxBitmap(expo_in_xpm) );
  append_item( _("expo:out"), wxBitmap(expo_out_xpm) );
  append_item( _("linear:in_out"), wxBitmap(linear_in_out_xpm) );
  append_item( _("linear:in"), wxBitmap(linear_in_xpm) );
  append_item( _("linear:out"), wxBitmap(linear_out_xpm) );
  append_item( _("none:in_out"), wxBitmap(none_in_out_xpm) );
  append_item( _("none:in"), wxBitmap(none_in_xpm) );
  append_item( _("none:out"), wxBitmap(none_out_xpm) );
  append_item( _("quad:in_out"), wxBitmap(quad_in_out_xpm) );
  append_item( _("quad:in"), wxBitmap(quad_in_xpm) );
  append_item( _("quad:out"), wxBitmap(quad_out_xpm) );
  append_item( _("quart:in_out"), wxBitmap(quart_in_out_xpm) );
  append_item( _("quart:in"), wxBitmap(quart_in_xpm) );
  append_item( _("quart:out"), wxBitmap(quart_out_xpm) );
  append_item( _("quint:in_out"), wxBitmap(quint_in_out_xpm) );
  append_item( _("quint:in"), wxBitmap(quint_in_xpm) );
  append_item( _("quint:out"), wxBitmap(quint_out_xpm) );
  append_item( _("sine:in_out"), wxBitmap(sine_in_out_xpm) );
  append_item( _("sine:in"), wxBitmap(sine_in_xpm) );
  append_item( _("sine:out"), wxBitmap(sine_out_xpm) );
} // easing_combo_popup::fill_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief An item of the list has been activated.
 * \param event The event sent by the list.
 */
void bf::easing_combo_popup::on_item_activated( wxListEvent& WXUNUSED(event) )
{
  wxCommandEvent e( wxEVT_COMMAND_COMBOBOX_SELECTED, GetId() );
  e.SetEventObject(this);
  ProcessEvent(e);
  Dismiss();
} // easing_combo_popup::on_item_activated()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::easing_combo_popup, wxListView)
END_EVENT_TABLE()
