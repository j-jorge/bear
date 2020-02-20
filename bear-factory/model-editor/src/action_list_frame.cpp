/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_list_frame class.
 * \author Sébastien Angibaud
 */
#include "bf/action_list_frame.hpp"

#include "bf/gui_model.hpp"
#include "bf/model_frame.hpp"
#include "bf/action_properties_frame.hpp"
#include "bf/wx_facilities.hpp"

#include "bf/history/action_add_action.hpp"
#include "bf/history/action_remove_action.hpp"
#include "bf/history/action_modify_action.hpp"

#include <vector>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The frame owning this one.
 */
bf::action_list_frame::action_list_frame( wxWindow* parent )
  : wxPanel( parent ), m_model_frame(NULL), m_action_list(NULL),
    m_new_action_button(NULL), m_delete_action_button(NULL),
    m_copy_action_button(NULL), m_properties_action_button(NULL)
{
  create_controls();
  update_controls();
} // action_list_frame::action_list_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the model on which we work.
 * \param f The model frame in which we take the actions.
 */
void bf::action_list_frame::set_model_frame( model_frame* f )
{
  m_model_frame = f;
  fill();
  update_controls();
} // action_list_frame::set_model_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Select an action of the model.
 */
void bf::action_list_frame::select_action( const std::string& action_name )
{
  m_action_list->SetStringSelection( std_to_wx_string(action_name) );
} // action_list_frame::select()

/*----------------------------------------------------------------------------*/
/**
 * \brief The action of the model changes.
 */
void bf::action_list_frame::update_action()
{
  if ( m_model_frame != NULL )
    m_action_list->SetStringSelection
      ( std_to_wx_string
        ( m_model_frame->get_model().get_active_action().get_name() ) );
} // action_list_frame::update_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::action_list_frame::create_controls()
{
  create_member_controls();
  create_sizer_controls();
  update_controls();
} // action_list_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update enable/disable of the controls of the window.
 */
void bf::action_list_frame::update_controls()
{
  bool enable = false;

  if ( m_model_frame != NULL )
    {
      enable = !m_model_frame->empty();
      m_new_action_button->Enable(true);
    }
  else
    m_new_action_button->Enable(false);

  m_delete_action_button->Enable(enable);
  m_copy_action_button->Enable(enable);
  m_properties_action_button->Enable(enable);
} // action_list_frame::update_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls for which we keep a reference.
 */
void bf::action_list_frame::create_member_controls()
{
  m_action_list = new wxListBox(this, IDC_ACTION_LIST);

  m_new_action_button = new wxButton(this, wxID_NEW);
  m_delete_action_button = new wxButton(this, wxID_DELETE);
  m_copy_action_button = new wxButton(this, wxID_COPY);
  m_properties_action_button = new wxButton(this, wxID_PROPERTIES);
} // action_list_frame::create_member_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::action_list_frame::create_sizer_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

  // the sizer for the buttons
  wxBoxSizer* s_sizer = new wxBoxSizer( wxVERTICAL );

  s_sizer->Add( m_new_action_button, 0, wxEXPAND | wxALL, 0 );
  s_sizer->Add( m_delete_action_button, 0, wxEXPAND | wxALL, 0 );
  s_sizer->Add( m_copy_action_button, 0, wxEXPAND | wxALL, 0 );
  s_sizer->Add( m_properties_action_button, 0, wxEXPAND | wxALL, 0 );

  sizer->Add( m_action_list, 1, wxEXPAND );
  sizer->Add( s_sizer, 0, wxEXPAND );

  SetSizer(sizer);
} // action_list_frame::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear the list of actions.
 */
void bf::action_list_frame::clear()
{
  m_action_list->Clear();
} // action_list_frame::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the list with the actions of the model.
 */
void bf::action_list_frame::fill()
{
  if ( m_model_frame == NULL )
    clear();
  else
    {
      const gui_model& mdl = m_model_frame->get_model();
      std::vector<std::string> arr;
      model::const_action_iterator it;
      const model::const_action_iterator end_it = mdl.action_end();

      for ( it = mdl.action_begin(); it != end_it; ++it )
        arr.push_back( it->get_name() );

      std::sort( arr.begin(), arr.end() );

      std::size_t i;

      for ( i=0;
            ( i != arr.size() ) && (i != m_action_list->GetCount());
            ++i )
        m_action_list->SetString( i, std_to_wx_string( arr[i] ) );

      for ( ; i!=arr.size(); ++i )
        m_action_list->Append( std_to_wx_string( arr[i] ) );

      while ( i != m_action_list->GetCount() )
        m_action_list->Delete( i );

      if ( mdl.has_active_action() )
        select_action(mdl.get_active_action().get_name());
    }
} // action_list_frame::fill()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on a action menu entry.
 * \param event This event occured.
 */
void bf::action_list_frame::on_new_action( wxCommandEvent& WXUNUSED(event) )
{
  if (m_model_frame != NULL)
    {
      action_properties_frame dlg(this, & m_model_frame->get_model());

      if ( dlg.ShowModal() == wxID_OK )
        {
          m_model_frame->clear_selection();

          m_model_frame->do_action
            ( new action_add_action
              ( new action
                ( dlg.get_action_name(), dlg.get_action_duration(),
                  dlg.get_sound(),
                  dlg.get_auto_next() ) ) );

          m_model_frame->set_active_action(dlg.get_action_name());
          fill();
          update_controls();
        }
    }
} // action_list_frame::on_new_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the button "delete".
 * \param event This event occured.
 */
void bf::action_list_frame::on_delete( wxCommandEvent& WXUNUSED(event) )
{
  if (m_model_frame != NULL)
    if ( !m_model_frame->empty() )
      {
        m_model_frame->clear_selection();

        int index = m_action_list->GetSelection();
        std::string name;
        if ( m_model_frame->get_model().has_active_action() )
          name = m_model_frame->get_model().get_active_action().get_name();

        m_model_frame->reset_active_action();
        m_model_frame->do_action
          ( new action_remove_action( name ) );

        fill();

        if ( !m_model_frame->empty() )
          {
            if ( index < (int)m_action_list->GetCount() )
              m_model_frame->set_active_action
                (wx_to_std_string(m_action_list->GetString(index)));
            else
              m_model_frame->set_active_action
                (wx_to_std_string(m_action_list->GetString(index-1)));
          }

        if ( m_model_frame->get_model().has_active_action() )
          select_action
            (m_model_frame->get_model().get_active_action().get_name());

        update_controls();
      }
} // action_list_frame::on_delete()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the button "copy".
 * \param event This event occured.
 */
void bf::action_list_frame::on_copy( wxCommandEvent& WXUNUSED(event) )
{
  if (m_model_frame != NULL)
    if ( !m_model_frame->empty() )
      {
        if ( !m_model_frame->get_model().has_active_action() )
          return;

        action* new_action =
          new action(m_model_frame->get_model().get_active_action());
        new_action->set_name
          ( m_model_frame->get_model().create_unique_action_name
            (new_action->get_name()) );

        m_model_frame->do_action( new action_add_action( new_action ) );

        fill();
        update_controls();
      }
} // action_list_frame::on_copy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the button "Properties".
 * \param event This event occured.
 */
void bf::action_list_frame::on_show_properties
( wxCommandEvent& WXUNUSED(event) )
{
  int index = m_action_list->GetSelection();

  if (index != wxNOT_FOUND)
    {
      std::string a( wx_to_std_string( m_action_list->GetStringSelection() ) );
      action_properties_frame dlg(this, & m_model_frame->get_model());
      dlg.fill_from( m_model_frame->get_model().get_action(a) );

      if ( dlg.ShowModal() == wxID_OK )
        {
          m_model_frame->do_action
            ( new action_modify_action
              (& m_model_frame->get_model().get_action(a),
               dlg.get_action_name(), dlg.get_action_duration(),
               dlg.get_sound(),
               dlg.get_auto_next() ) );

          m_model_frame->set_active_action(dlg.get_action_name());
          fill();
        }
    }
} // model_list_frame::on_show_properties()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform all windows about the selected action.
 * \param event This event occured.
 */
void bf::action_list_frame::on_select_action( wxCommandEvent& event )
{
  if ( event.IsSelection() )
    if ( m_model_frame != NULL )
      {
        m_model_frame->set_active_action
          (wx_to_std_string(m_action_list->GetStringSelection()));
      }
} // action_list_frame::on_select_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when closing the window.
 * \param event This event occured.
 */
void bf::action_list_frame::on_close( wxCloseEvent& event )
{
  if ( event.CanVeto() )
    {
      Hide();
      event.Veto();
    }
} // action_list_frame::on_close()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::action_list_frame, wxPanel)
  EVT_BUTTON( wxID_NEW, bf::action_list_frame::on_new_action )
  EVT_BUTTON( wxID_DELETE, bf::action_list_frame::on_delete )
  EVT_BUTTON( wxID_COPY, bf::action_list_frame::on_copy )
  EVT_LISTBOX( bf::action_list_frame::IDC_ACTION_LIST,
               bf::action_list_frame::on_select_action )
  EVT_BUTTON( wxID_PROPERTIES, bf::action_list_frame::on_show_properties )
  EVT_LISTBOX_DCLICK( bf::action_list_frame::IDC_ACTION_LIST,
                      bf::action_list_frame::on_show_properties )
  EVT_CLOSE( bf::action_list_frame::on_close )
END_EVENT_TABLE()
