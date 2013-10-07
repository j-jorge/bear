/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::value_editor_dialog class.
 * \author Julien Jorge
 */

#include "bf/default_value.hpp"
#include "bf/human_readable.hpp"
#include "bf/wx_facilities.hpp"

#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/button.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param title The title of the dialog.
 * \param v The initial value.
 */
template<typename Editor, typename Type>
bf::value_editor_dialog<Editor, Type>::value_editor_dialog
( wxWindow& parent, const wxString& title, const value_type& v )
  : wxDialog( &parent, wxID_ANY, title,
              wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ), m_value(v)
{
  m_editor = new editor_type( *this, m_value );
  init();
} // value_editor_dialog::value_editor_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param title The title of the dialog.
 * \param v The initial value.
 * \param env The workspace environment to use.
 */
template<typename Editor, typename Type>
bf::value_editor_dialog<Editor, Type>::value_editor_dialog
( wxWindow& parent, const wxString& title, const value_type& v,
  workspace_environment* env )
  : wxDialog( &parent, wxID_ANY, title,
              wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ), m_value(v)
{
  m_editor = new editor_type( *this, env, m_value );
  init();
} // value_editor_dialog::value_editor_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param title The title of the dialog.
 * \param values The valid values for the field.
 * \param v The initial value.
 */
template<typename Editor, typename Type>
bf::value_editor_dialog<Editor, Type>::value_editor_dialog
( wxWindow& parent, const wxString& title, const wxArrayString& values,
  const value_type& v )
  : wxDialog( &parent, wxID_ANY, title ), m_value(v)
{
  m_editor = new editor_type( *this, values, m_value );
  init();
} // value_editor_dialog::value_editor_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param title The title of the dialog.
 * \param min The minimum valid value.
 * \param max The maximum valid value.
 * \param v The initial value.
 */
template<typename Editor, typename Type>
bf::value_editor_dialog<Editor, Type>::value_editor_dialog
( wxWindow& parent, const wxString& title, const value_type& min,
  const value_type& max, const value_type& v )
  : wxDialog( &parent, wxID_ANY, title ), m_value(v)
{
  m_editor = new editor_type( *this, min, max, m_value );
  init();
} // value_editor_dialog::value_editor_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current value.
 */
template<typename Editor, typename Type>
const typename bf::value_editor_dialog<Editor, Type>::value_type&
bf::value_editor_dialog<Editor, Type>::get_value() const
{
  return m_value;
} // value_editor_dialog::get_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the current value.
 * \param v The new value.
 */
template<typename Editor, typename Type>
void bf::value_editor_dialog<Editor, Type>::set_value( const value_type& v )
{
  m_value = v;
  m_editor->set_value(m_value);
} // value_editor_dialog::set_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the editor created in this dialog.
 */
template<typename Editor, typename Type>
typename bf::value_editor_dialog<Editor, Type>::editor_type&
bf::value_editor_dialog<Editor, Type>::get_editor()
{
  return *m_editor;
} // value_editor_dialog::get_editor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the dialog.
 */
template<typename Editor, typename Type>
void bf::value_editor_dialog<Editor, Type>::init()
{
  create_sizers();

  this->Fit();

  this->Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
                 wxCommandEventHandler(self_type::on_ok) );

  m_editor->SetFocus();
} // value_editor_dialog::init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizers containing the controls.
 */
template<typename Editor, typename Type>
void bf::value_editor_dialog<Editor, Type>::create_sizers()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  sizer->Add( m_editor, 1, wxALL | wxEXPAND, 5 );
  sizer->Add( CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0,
              wxALL | wxCENTER, 5 );

  SetSizer(sizer);
} // value_editor_dialog::create_sizers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event handler when the user clicks on the "ok" button.
 * \param event The event.
 */
template<typename Editor, typename Type>
void bf::value_editor_dialog<Editor, Type>::on_ok
( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_editor->validate() )
    {
      m_value = m_editor->get_value();
      this->EndModal(wxID_OK);
    }
  else
    {
      wxMessageDialog dlg
        ( this, _("The value is not valid for this type."),
          _("Invalid value"), wxOK );

      dlg.ShowModal();
    }
} // value_editor_dialog::on_ok()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param type The name of the edited type.
 * \param v The initial value.
 */
template<typename Editor, typename T>
bf::value_editor_dialog< Editor, std::list<T> >::value_editor_dialog
( wxWindow& parent, const wxString& type, const value_type& v )
  : wxDialog( &parent, wxID_ANY, _("List of '") + type + wxT("'"),
              wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ), m_value(v)
{
  m_dialog = new dialog_type( *this, type, default_value<T>::get() );

  init();
  fill();
} // value_editor_dialog::value_editor_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param type The name of the edited type.
 * \param v The initial value.
 * \param env The workspace environment to use.
 */
template<typename Editor, typename T>
bf::value_editor_dialog< Editor, std::list<T> >::value_editor_dialog
( wxWindow& parent, const wxString& type, const value_type& v,
  workspace_environment* env )
  : wxDialog( &parent, wxID_ANY, _("List of '") + type + wxT("'"),
              wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ), m_value(v)
{
  m_dialog = new dialog_type( *this, type, default_value<T>::get(), env );

  init();
  fill();
} // value_editor_dialog::value_editor_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param type The name of the edited type.
 * \param values The valid values for the field.
 * \param v The initial value.
 */
template<typename Editor, typename T>
bf::value_editor_dialog< Editor, std::list<T> >::value_editor_dialog
( wxWindow& parent, const wxString& type, const wxArrayString& values,
  const value_type& v )
  : wxDialog( &parent, wxID_ANY, _("List of '") + type + wxT("'"),
              wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ), m_value(v)
{
  m_dialog = new dialog_type( *this, type, values, default_value<T>::get() );

  init();
  fill();
} // value_editor_dialog::value_editor_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param type The name of the edited type.
 * \param min The minimum valid value.
 * \param max The maximum valid value.
 * \param v The initial value.
 */
template<typename Editor, typename T>
bf::value_editor_dialog< Editor, std::list<T> >::value_editor_dialog
( wxWindow& parent, const wxString& type, const T& min, const T& max,
  const value_type& v )
  : wxDialog( &parent, wxID_ANY, _("List of '") + type + wxT("'"),
              wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ), m_value(v)
{
  m_dialog = new dialog_type( *this, type, min, max, default_value<T>::get() );

  init();
  fill();
} // value_editor_dialog::value_editor_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current value.
 */
template<typename Editor, typename T>
const typename bf::value_editor_dialog< Editor, std::list<T> >::value_type&
bf::value_editor_dialog< Editor, std::list<T> >::get_value() const
{
  return m_value;
} // value_editor_dialog::get_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the current value.
 * \param v The new value.
 */
template<typename Editor, typename T>
void bf::value_editor_dialog< Editor, std::list<T> >::set_value
( const value_type& v )
{
  m_value = v;
  fill();
} // value_editor_dialog::set_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the dialog.
 */
template<typename Editor, typename T>
void bf::value_editor_dialog< Editor, std::list<T> >::init()
{
  create_controls();
  create_sizers();
  this->Fit();

  this->Connect( wxID_UP, wxEVT_COMMAND_BUTTON_CLICKED,
                 wxCommandEventHandler(self_type::on_up) );
  this->Connect( wxID_DOWN, wxEVT_COMMAND_BUTTON_CLICKED,
                 wxCommandEventHandler(self_type::on_down) );
  this->Connect( wxID_NEW, wxEVT_COMMAND_BUTTON_CLICKED,
                 wxCommandEventHandler(self_type::on_new) );
  this->Connect( wxID_EDIT, wxEVT_COMMAND_BUTTON_CLICKED,
                 wxCommandEventHandler(self_type::on_edit) );
  this->Connect( wxID_DELETE, wxEVT_COMMAND_BUTTON_CLICKED,
                 wxCommandEventHandler(self_type::on_delete) );
  this->Connect( m_list->GetId(), wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,
                 wxCommandEventHandler(self_type::on_edit) );
} // value_editor_dialog::init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
template<typename Editor, typename T>
void bf::value_editor_dialog< Editor, std::list<T> >::create_controls()
{
  m_list = new wxListBox( this, wxID_ANY );
} // value_editor_dialog::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizers containing the controls and the controls for which
 *        we do not keep a pointer.
 */
template<typename Editor, typename T>
void bf::value_editor_dialog< Editor, std::list<T> >::create_sizers()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

  // the sizer for the buttons
  wxBoxSizer* s_sizer = new wxBoxSizer( wxVERTICAL );

  s_sizer->Add( new wxButton(this, wxID_NEW), 1, wxEXPAND | wxALL, 0 );
  s_sizer->Add( new wxButton(this, wxID_EDIT), 1, wxEXPAND | wxALL, 0 );
  s_sizer->Add( new wxButton(this, wxID_DELETE), 1, wxEXPAND | wxALL, 0 );
  s_sizer->Add( new wxButton(this, wxID_UP), 1, wxEXPAND | wxALL, 0 );
  s_sizer->Add( new wxButton(this, wxID_DOWN), 1, wxEXPAND | wxALL, 0 );

  sizer->Add( m_list, 1, wxEXPAND );
  sizer->Add( s_sizer, 0, wxEXPAND );

  s_sizer = new wxBoxSizer( wxVERTICAL );
  s_sizer->Add( sizer, 1, wxEXPAND );
  s_sizer->Add
    ( CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxCENTER, 5 );

  SetSizer(s_sizer);
} // value_editor_dialog::create_sizers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Display the value in the list control.
 */
template<typename Editor, typename T>
void bf::value_editor_dialog< Editor, std::list<T> >::fill()
{
  int index = m_list->GetSelection();

  m_list->Clear();

  typename value_type::const_iterator it;

  for (it=m_value.begin(); it!=m_value.end(); ++it)
    m_list->Append( human_readable<T>::convert(*it) );

  m_list->SetSelection(index);
} // value_editor_dialog::fill()

/*----------------------------------------------------------------------------*/
/**
 * \brief Edit a value of the list.
 * \param index The index of the edited value in the list.
 */
template<typename Editor, typename T>
void bf::value_editor_dialog< Editor, std::list<T> >::edit_value
( unsigned int index )
{
  typename value_type::iterator it = m_value.begin();

  std::advance(it, index);
  m_dialog->set_value(*it);

  if ( m_dialog->ShowModal() == wxID_OK )
    {
      *it = m_dialog->get_value();
      fill();
    }
} // value_editor_dialog::edit_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "Up" button.
 * \param event The event.
 */
template<typename Editor, typename T>
void bf::value_editor_dialog< Editor, std::list<T> >::on_up
( wxCommandEvent& WXUNUSED(event) )
{
  int index = m_list->GetSelection();

  if ( index != wxNOT_FOUND )
    if ( index > 0 )
      {
        typename value_type::iterator prec = m_value.begin();

        std::advance(prec, index-1);

        typename value_type::iterator it(prec);
        ++it;

        std::swap(*it, *prec);
        m_list->SetSelection(index-1);

        fill();
      }
} // value_editor_dialog::on_up()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "Down" button.
 * \param event The event.
 */
template<typename Editor, typename T>
void bf::value_editor_dialog< Editor, std::list<T> >::on_down
( wxCommandEvent& WXUNUSED(event) )
{
  int index = m_list->GetSelection();

  if ( index != wxNOT_FOUND )
    if ( (unsigned int)index + 1 < m_list->GetCount() )
      {
        typename value_type::iterator it = m_value.begin();

        std::advance(it, index);

        typename value_type::iterator succ(it);
        ++succ;

        std::swap(*it, *succ);
        m_list->SetSelection(index+1);

        fill();
      }
} // value_editor_dialog::on_down()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "New" button.
 * \param event The event.
 */
template<typename Editor, typename T>
void bf::value_editor_dialog< Editor, std::list<T> >::on_new
( wxCommandEvent& WXUNUSED(event) )
{
  m_dialog->set_value( default_value<T>::get() );

  if ( m_dialog->ShowModal() == wxID_OK )
    {
      m_value.push_back( m_dialog->get_value() );
      fill();
    }
} // value_editor_dialog::on_new()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "Edit" button.
 * \param event The event.
 */
template<typename Editor, typename T>
void bf::value_editor_dialog< Editor, std::list<T> >::on_edit
( wxCommandEvent& WXUNUSED(event) )
{
  int index = m_list->GetSelection();

  if ( index != wxNOT_FOUND )
    edit_value(index);
} // value_editor_dialog::on_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "Delete" button.
 * \param event The event.
 */
template<typename Editor, typename T>
void bf::value_editor_dialog< Editor, std::list<T> >::on_delete
( wxCommandEvent& WXUNUSED(event) )
{
  int index = m_list->GetSelection();

  if ( index != wxNOT_FOUND )
    {
      typename value_type::iterator it = m_value.begin();

      std::advance(it, index);
      m_value.erase(it);

      if ( !m_value.empty() )
        if ( (unsigned int)index == m_value.size() )
          m_list->SetSelection(index-1);

      fill();
    }
} // value_editor_dialog::on_delete()
