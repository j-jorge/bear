/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::error_check_level_dialog class.
 * \author Sebastien Angibaud
 */
#include "bf/error_check_level_dialog.hpp"

#include "bf/item_event.hpp"
#include "bf/item_check_result.hpp"
#include "bf/layer.hpp"
#include "bf/layer_check_result.hpp"
#include "bf/level_check_result.hpp"
#include "bf/wx_facilities.hpp"

#include <wx/sizer.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param id The identifier of the window.
 */
bf::error_check_level_dialog::error_check_level_dialog
( wxWindow* parent, wxWindowID id )
  : wxFrame( parent, id, _("Level errors") )
{
  create_controls();
  create_sizers();
} // error_check_level_dialog::error_check_level_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the errors to display.
 * \param errors The errors.
 */
void bf::error_check_level_dialog::set_errors
( const level_check_result& errors )
{
  m_items.clear();

  for ( level_check_result::layer_iterator it=errors.layer_begin();
          it!=errors.layer_end(); ++it )
      fill_layer_check_result( it->first, it->second );
} // error_check_level_dialog::set_errors()

/*----------------------------------------------------------------------------*/
/**
 * \brief Display the check errors of a layer.
 * \param lay The checked layer.
 * \param result The result of the check.
 */
void bf::error_check_level_dialog::fill_layer_check_result
( layer* lay, const layer_check_result& result )
{
  wxString layer_name = std_to_wx_string(lay->get_displayed_name());

  for ( layer_check_result::item_iterator it=result.item_begin();
      it!=result.item_end(); ++it )
    fill_item_check_result( it->first, it->second, layer_name );
} // error_check_level_dialog::fill_layer_check_result()

/*----------------------------------------------------------------------------*/
/**
 * \brief Display the check errors of an item.
 * \param item The checked item.
 * \param result The result of the check.
 * \param layer_name The name of the layer containing \a item.
 */
void bf::error_check_level_dialog::fill_item_check_result
( item_instance* item, const item_check_result& result,
  const wxString& layer_name )
{
  for ( item_check_result::const_iterator it=result.begin();
      it!=result.end(); ++it )
    {
      m_items.push_back(item);

      wxListItem entry;
      entry.SetId(m_items_list->GetItemCount());

      entry.SetColumn(0);
      entry.SetText( std_to_wx_string(it->get_cause()) );
      m_items_list->InsertItem(entry);

      entry.SetColumn(1);
      entry.SetText( std_to_wx_string(it->get_message()) );
      m_items_list->SetItem(entry);

      entry.SetColumn(2);
      entry.SetText(layer_name);
      m_items_list->SetItem(entry);
    }
} // error_check_level_dialog::fill_item_check_result()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
void bf::error_check_level_dialog::create_controls()
{
  m_items_list =
      new wxListView
      ( this, ID_ERROR_LIST, wxDefaultPosition, wxDefaultSize,
          wxLC_REPORT | wxLC_VRULES | wxLC_SINGLE_SEL );

  m_items_list->InsertColumn(0, _("Cause"));
  m_items_list->InsertColumn(1, _("Error message"));
  m_items_list->InsertColumn(2, _("Layer"));

  m_details =
      new wxTextCtrl
      (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
          wxTE_MULTILINE | wxTE_READONLY );
} // error_check_level_dialog::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
void bf::error_check_level_dialog::create_sizers()
{
   wxBoxSizer* v_sizer = new wxBoxSizer( wxVERTICAL );

   v_sizer->Add( m_items_list, 3, wxEXPAND );
   v_sizer->Add( m_details, 1, wxEXPAND );
   SetSizer(v_sizer);
} // error_check_level_dialog::create_sizers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Ajust the size of the last column so there is no empty space on the
 *        right.
 */
void bf::error_check_level_dialog::adjust_last_column_size() const
{
  m_items_list->SetColumnWidth(0, wxLIST_AUTOSIZE);
  m_items_list->SetColumnWidth(2, wxLIST_AUTOSIZE);

  m_items_list->SetColumnWidth
    ( 1, m_items_list->GetSize().x - m_items_list->GetColumnWidth(0) -
      m_items_list->GetColumnWidth(2) );
} // error_check_level_dialog::adjust_last_column_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when closing the window.
 * \param event This event occurred.
 */
void bf::error_check_level_dialog::on_close(wxCloseEvent& event)
{
  if ( event.CanVeto() )
    {
      Hide();
      event.Veto();
    }
  else
    Destroy();
} // error_check_level_dialog::on_close()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent to a resized window.
 * \param event The event.
 */
void bf::error_check_level_dialog::on_size(wxSizeEvent& event)
{
  adjust_last_column_size();
  event.Skip();
} // error_check_level_dialog::on_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief A line of the error list has been selected.
 * \param event The event.
 */
void bf::error_check_level_dialog::on_error_selected(wxListEvent& event)
{
  wxListItem line;
  line.SetId(event.GetIndex());
  line.SetColumn(1);
  m_items_list->GetItem(line);

  m_details->SetValue( line.GetText() );

  item_event selection_event
    ( m_items[event.GetIndex()], item_event::selection_event_type, GetId() );
  selection_event.SetEventObject(this);
  ProcessEvent(selection_event);
} // error_check_level_dialog::on_error_selected()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::error_check_level_dialog, wxFrame)
  EVT_CLOSE( bf::error_check_level_dialog::on_close )
  EVT_SIZE( bf::error_check_level_dialog::on_size )

  EVT_LIST_ITEM_SELECTED( bf::error_check_level_dialog::ID_ERROR_LIST,
      bf::error_check_level_dialog::on_error_selected )
END_EVENT_TABLE()
