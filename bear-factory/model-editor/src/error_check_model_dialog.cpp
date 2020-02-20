/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::error_check_model_dialog class.
 * \author Sebastien Angibaud
 */
#include "bf/error_check_model_dialog.hpp"

#include "bf/action.hpp"
#include "bf/wx_facilities.hpp"

#include <wx/sizer.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param actions Invalid actions.
 */
bf::error_check_model_dialog::error_check_model_dialog
( wxWindow& parent,
  const std::list< const action* >& actions )
  : wxDialog( &parent, wxID_ANY, _("Bad action"), wxDefaultPosition,
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ),
    m_actions(actions)
{
  create_controls();
  create_sizers();
} // error_check_model_dialog::error_check_model_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the selected layer.
 */
unsigned int bf::error_check_model_dialog::get_index_selected() const
{
  long index = m_actions_list->GetFocusedItem();

  if ( index == wxNOT_FOUND )
    index = 0;

  return (unsigned int)index;
} // error_check_model_dialog::get_index_selected()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
void bf::error_check_model_dialog::create_controls()
{
  m_actions_list = new wxListView
    ( this, wxID_ANY, wxDefaultPosition,
      wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_VRULES ,
      wxDefaultValidator, _("bad actions") );

  m_actions_list->InsertColumn(0, wxT("Action"));
  m_actions_list->InsertColumn(1, wxT("Error"));

  std::list< const action* >::const_iterator it;
  unsigned int i=0;

  for ( it = m_actions.begin(); it != m_actions.end(); ++it, ++i)
    {
      m_actions_list->InsertItem(i, std_to_wx_string((*it)->get_name()));
      wxListItem prop;
      prop.SetId(i);
      m_actions_list->GetItem(prop);
      prop.SetText(_("The next action does not exist."));
      prop.SetColumn(1);
      m_actions_list->SetItem(prop);
    }

  m_actions_list->Select(0);
} // error_check_model_dialog::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
void bf::error_check_model_dialog::create_sizers()
{
   wxBoxSizer* v_sizer = new wxBoxSizer( wxVERTICAL );

   v_sizer->Add( m_actions_list, 3, wxALL | wxEXPAND, 5 );
   v_sizer->AddSpacer(5);
   v_sizer->Add( CreateStdDialogButtonSizer(wxOK), 0,
                 wxHORIZONTAL | wxCENTER , 500 );
   SetSizer(v_sizer);
} // error_check_model_dialog::create_sizers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Ajust the size of the last column so there is no empty space on the
 *        right.
 */
void bf::error_check_model_dialog::adjust_last_column_size()
{
  m_actions_list->SetColumnWidth
    ( 1, m_actions_list->GetSize().x - m_actions_list->GetColumnWidth(0) );
} // error_check_model_dialog::adjust_last_column_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent to a resized window.
 * \param event The event.
 */
void bf::error_check_model_dialog::on_size(wxSizeEvent& event)
{
  adjust_last_column_size();
  event.Skip();
} // error_check_model_dialog::on_size()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::error_check_model_dialog, wxDialog)
  EVT_SIZE( bf::error_check_model_dialog::on_size )
END_EVENT_TABLE()
