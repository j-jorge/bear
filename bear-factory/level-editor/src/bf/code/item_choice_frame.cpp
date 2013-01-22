/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::item_choice_frame class.
 * \author Sebastien Angibaud
 */
#include "bf/item_choice_frame.hpp"

#include "bf/item_class.hpp"
#include "bf/item_instance.hpp"
#include "bf/item_selection.hpp"
#include "bf/wx_facilities.hpp"

#include <wx/toolbar.h>
#include <wx/artprov.h>
#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this window.
 * \param selected_group The items already selected.
 * \param choice The items choice.
 */
bf::item_choice_frame::item_choice_frame
( wxWindow* parent, const item_selection& selected_group,
  const std::set<item_instance*>& choice )
  : wxDialog(parent, wxID_ANY, wxString(_("Items choice"))),
    m_choice(choice), m_selected_group(selected_group)
{
  create_controls();
  Fit();
} // item_choice_frame::item_choice_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the selected items.
 */
void bf::item_choice_frame::get_selection
(std::set<item_instance*>& selected_items)
{
  std::set<item_instance*>::const_iterator it;
  unsigned int i;

  for ( i=0, it=m_choice.begin();
        i!=m_item_checklistbox->GetCount(); ++i, ++it )
    if ( m_item_checklistbox->IsChecked(i)
         != m_selected_group.is_selected(*it) )
      selected_items.insert(*it);
} // item_choice_frame::get_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::item_choice_frame::create_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  m_all_button = new wxButton(this, IDC_SELECT_ALL, _("Select all"));
  m_inverse_button = new wxButton(this, IDC_INVERSE, _("Inverse selection"));

  wxBoxSizer* h_sizer = new wxBoxSizer( wxHORIZONTAL );

  h_sizer->Add( m_all_button, 0,
                wxALL | wxCENTER, 5 );
  h_sizer->Add( m_inverse_button, 0,
                wxALL | wxCENTER, 5 );

  sizer->Add( h_sizer, 0,
              wxALL | wxCENTER, 5 );

  sizer->Add( create_list_sizer(), 0, wxEXPAND );

  sizer->Add( CreateStdDialogButtonSizer(wxOK |wxCANCEL), 0,
              wxALL | wxCENTER, 5 );

  SetSizer(sizer);
} // item_choice_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and the sizer for the item list.
 */
wxSizer* bf::item_choice_frame::create_list_sizer()
{
  wxStaticBoxSizer* result =
    new wxStaticBoxSizer( wxHORIZONTAL, this, _("Choice") );

  m_item_checklistbox = new wxCheckListBox( this, IDC_LAYER_LIST );

  std::set<item_instance*>::iterator it;
  unsigned int i;
  for ( it = m_choice.begin(), i=0; it != m_choice.end(); ++it, ++i)
    {
      std::ostringstream stm;
      stm << "class=" << (*it)->get_class().get_class_name() << ", "
          << "pos=[" << (*it)->get_rendering_parameters().get_left() << ", "
          << (*it)->get_rendering_parameters().get_bottom() << "] "
          << "size=[" << (*it)->get_rendering_parameters().get_width() << ", "
          << (*it)->get_rendering_parameters().get_height() << "]";

      if ( !(*it)->get_id().empty() )
        stm << " id='" << (*it)->get_id() << "'";

      wxString str = std_to_wx_string(stm.str());

      m_item_checklistbox->Append(str);
      m_item_checklistbox->Check(i, m_selected_group.is_selected(*it));
    }

  result->Add( m_item_checklistbox, 0, wxALL, 5 );

  return result;
} // item_choice_frame::create_list_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inverse the selection.
 */
void bf::item_choice_frame::inverse_selection()
{
  std::set<item_instance*>::iterator it;
  unsigned int i;
  for ( it = m_choice.begin(), i=0; it != m_choice.end(); ++it, ++i)
    m_item_checklistbox->Check(i, !m_item_checklistbox->IsChecked(i));
} // item_choice_frame::inverse_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Select all items.
 */
void bf::item_choice_frame::select_all()
{
  std::set<item_instance*>::iterator it;
  unsigned int i;
  for ( it = m_choice.begin(), i=0; it != m_choice.end(); ++it, ++i)
    m_item_checklistbox->Check(i, true);
} // item_choice_frame::select_all()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "OK" button.
 * \param event This event occured.
 */
void bf::item_choice_frame::on_ok( wxCommandEvent& WXUNUSED(event) )
{
  EndModal(wxID_OK);
} // item_choice_frame::on_ok()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "Inverse" button.
 * \param event This event occured.
 */
void bf::item_choice_frame::on_inverse( wxCommandEvent& WXUNUSED(event)  )
{
  inverse_selection();
} // item_choice_frame::on_inverse()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "Select all" button.
 * \param event This event occured.
 */
void bf::item_choice_frame::on_select_all( wxCommandEvent& WXUNUSED(event) )
{
  select_all();
} // item_choice_frame::on_selectall()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::item_choice_frame, wxDialog)
  EVT_BUTTON( wxID_OK, bf::item_choice_frame::on_ok )
  EVT_BUTTON( IDC_SELECT_ALL, bf::item_choice_frame::on_select_all )
  EVT_BUTTON( IDC_INVERSE, bf::item_choice_frame::on_inverse )
END_EVENT_TABLE()
