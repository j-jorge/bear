/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::item_class_selection_dialog class.
 * \author Julien Jorge
 */
#include "bf/item_class_selection_dialog.hpp"

#include "bf/class_tree_ctrl.hpp"
#include "bf/wx_facilities.hpp"

#include <wx/sizer.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param pool The classes to display in the tree.
 * \param parent The parent class.
 * \param class_name The initial value.
 */
bf::item_class_selection_dialog::item_class_selection_dialog
( const item_class_pool& pool, wxWindow* parent, const std::string& class_name )
  : wxDialog(parent, wxID_ANY, _("Select an item class"), wxDefaultPosition,
             wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ),
    m_class_name(class_name)
{
  m_tree = new class_tree_ctrl(pool, this, wxID_ANY);

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

  sizer->Add(m_tree, 1, wxEXPAND);
  sizer->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND);

  SetSizer(sizer);

  Connect
    ( m_tree->GetId(), class_selected_event::class_selected_event_type,
      class_selected_event_handler
      (item_class_selection_dialog::on_class_selected) );
} // item_class_selection_dialog::item_class_selection_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the selected item class.
 */
const std::string& bf::item_class_selection_dialog::get_class_name() const
{
  return m_class_name;
} // item_class_selection_dialog::get_class_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Validate the selection.
 * \param event This event occured.
 */
void bf::item_class_selection_dialog::on_ok( wxCommandEvent& WXUNUSED(event) )
{
  wxTreeItemId item = m_tree->GetSelection();

  if ( item.IsOk() )
    if ( !m_tree->ItemHasChildren(item) )
      {
        m_class_name = wx_to_std_string( m_tree->GetItemText(item) );
        EndModal(wxID_OK);
      }
} // item_class_selection_dialog::on_ok()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when the user picks a class in the class list.
 * \param event This event occured.
 */
void bf::item_class_selection_dialog::on_class_selected
( class_selected_event& event )
{
  m_class_name = event.get_class_name();

  EndModal(wxID_OK);
} // item_class_selection_dialog::on_class_selected()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::item_class_selection_dialog, wxDialog)
  EVT_BUTTON( wxID_OK, bf::item_class_selection_dialog::on_ok )
END_EVENT_TABLE()
