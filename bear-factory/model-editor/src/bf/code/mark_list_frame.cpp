/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::mark_list_frame class.
 * \author Sébastien Angibaud
 */
#include "bf/mark_list_frame.hpp"

#include "bf/gui_model.hpp"
#include "bf/model_frame.hpp"
#include "bf/mark_properties_frame.hpp"
#include "bf/workspace_environment.hpp"
#include "bf/wx_facilities.hpp"

#include "bf/history/action_add_mark.hpp"
#include "bf/history/action_remove_mark.hpp"
#include "bf/history/action_modify_mark.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The frame owning this one.
 * \param env The workspace environment to use.
 */
bf::mark_list_frame::mark_list_frame
( wxWindow* parent, workspace_environment* env )
  : wxPanel( parent ), m_model_frame(NULL), m_mark_list(NULL),
    m_new_mark_button(NULL), m_delete_mark_button(NULL),
    m_properties_mark_button(NULL), m_workspace(env)
{
  create_controls();
  update_controls();
} // mark_list_frame::mark_list_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the model on which we work.
 * \param f The model frame in which we take the actions.
 */
void bf::mark_list_frame::set_model_frame( model_frame* f )
{
  m_model_frame = f;
  if ( m_model_frame )
    m_workspace = m_model_frame->get_workspace();

  fill();
  update_controls();
} // mark_list_frame::set_model_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update when the action is modified.
 */
void bf::mark_list_frame::update_action()
{
  fill();
} // mark_list_frame::update_action()


/*----------------------------------------------------------------------------*/
/**
 * \brief Update when the current selected mark is modified.
 */
void bf::mark_list_frame::update_mark()
{
  fill();
} // mark_list_frame::update_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Select an mark of the model.
 */
void bf::mark_list_frame::select_mark( const std::string& mark_name )
{
  m_model_frame->set_mark_selection(mark_name);
} // mark_list_frame::select()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::mark_list_frame::create_controls()
{
  create_member_controls();
  create_sizer_controls();
} // mark_list_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update enable/disable of the controls of the window.
 */
void bf::mark_list_frame::update_controls()
{
  bool enable = false;
  bool new_mark_enable = false;

  if ( m_model_frame != NULL )
    if ( m_model_frame->get_model().has_active_action() )
      {
        enable = m_model_frame->get_model().get_active_action().has_marks();
        new_mark_enable = true;
      }

  m_new_mark_button->Enable(new_mark_enable);
  m_delete_mark_button->Enable(enable);
  m_properties_mark_button->Enable(enable);
} // action_list_frame::update_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls for which we keep a reference.
 */
void bf::mark_list_frame::create_member_controls()
{
  m_mark_list = new wxListBox( this, IDC_MARK_LIST );
  m_new_mark_button = new wxButton(this, wxID_NEW);

  m_new_mark_button->SetLabel(wxEmptyString);
} // mark_list_frame::create_member_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::mark_list_frame::create_sizer_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

  // the sizer for the buttons
  wxBoxSizer* s_sizer = new wxBoxSizer( wxVERTICAL );

  m_delete_mark_button = new wxButton(this, wxID_DELETE);
  m_properties_mark_button = new wxButton(this, wxID_PROPERTIES);

  s_sizer->Add( m_new_mark_button, 0, wxEXPAND | wxALL, 0 );
  s_sizer->Add( m_delete_mark_button, 0, wxEXPAND | wxALL, 0 );
  s_sizer->Add( m_properties_mark_button, 0, wxEXPAND | wxALL, 0 );

  sizer->Add( m_mark_list, 1, wxEXPAND );
  sizer->Add( s_sizer, 0, wxEXPAND );

  SetSizer(sizer);
} // mark_list_frame::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear the list of marks.
 */
void bf::mark_list_frame::clear()
{
  m_mark_list->Clear();
} // mark_list_frame::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the list with the marks of the model.
 */
void bf::mark_list_frame::fill()
{
  clear();

  if ( m_model_frame != NULL )
    if ( m_model_frame->get_model().has_active_action() )
      {
        const action& a = m_model_frame->get_model().get_active_action();

        action::const_mark_iterator it;

        wxArrayString arr;

        for ( it = a.mark_begin(); it != a.mark_end(); ++it) 
          arr.Add( std_to_wx_string( it->get_label() ) );

        arr.Sort();
        m_mark_list->Set(arr);

        if ( m_model_frame->get_model().has_mark_selection() )
          {
            const mark* m
              ( m_model_frame->get_model().get_main_mark_selection() );

            m_mark_list->SetStringSelection( std_to_wx_string(m->get_label()) );
          }
      }
} // mark_list_frame::fill()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on a mark menu entry.
 * \param event This event occured.
 */
void bf::mark_list_frame::on_new_mark(wxCommandEvent& WXUNUSED(event))
{
  if ( m_model_frame != NULL )
    if ( m_model_frame->get_model().has_active_action() )
      {
        const action& a = m_model_frame->get_model().get_active_action();
        mark_properties_frame dlg(this, &a, m_workspace);

        if ( dlg.ShowModal() == wxID_OK )
          {
            mark* m = new mark();

            m->set_label(dlg.get_mark_name());
            m->set_animation(dlg.get_mark_animation());
            m->apply_angle_to_animation(dlg.apply_angle_to_animation());
            m->pause_animation_when_hidden(dlg.pause_animation_when_hidden());
            m->reset_animation_with_action(dlg.reset_animation_with_action());

            m_model_frame->do_action
              ( new action_add_mark(a.get_name(), m) );
            fill();

            select_mark(dlg.get_mark_name());
            update_controls();
          }
      }
} // mark_list_frame::on_new_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the button "delete".
 * \param event This event occured.
 */
void bf::mark_list_frame::on_delete(wxCommandEvent& WXUNUSED(event))
{
  if ( m_model_frame != NULL )
    if ( m_model_frame->get_model().has_active_action() )
      {
        int index = m_mark_list->GetSelection();
        if ( index != wxNOT_FOUND )
          {
            action& a = m_model_frame->get_model().get_active_action();
            std::string selection =
              wx_to_std_string(m_mark_list->GetStringSelection());
            m_model_frame->clear_selection();

            m_model_frame->do_action
              ( new action_remove_mark
                ( m_model_frame->get_model().get_active_action().get_name(),
                  a.get_mark(selection)));

            fill();

            if ( a.mark_begin() != a.mark_end() )
              {
          if ( index < (int)m_mark_list->GetCount() )
            m_mark_list->Select(index);
          else
            m_mark_list->Select(index-1);

          select_mark
            (wx_to_std_string(m_mark_list->GetStringSelection()));
              }
          }
        update_controls();
      }
} // mark_list_frame::on_delete()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the button "Properties".
 * \param event This event occured.
 */
void bf::mark_list_frame::on_show_properties(wxCommandEvent& WXUNUSED(event))
{
  if ( m_model_frame != NULL )
    if ( m_model_frame->get_model().has_active_action() )
      {
        int index = m_mark_list->GetSelection();

        if (index != wxNOT_FOUND)
          {
            action& a = m_model_frame->get_model().get_active_action();
            mark* m = a.get_mark
              (wx_to_std_string(m_mark_list->GetStringSelection()));

            mark_properties_frame dlg(this, &a, m_workspace );
            dlg.fill_from( *m );

            if ( dlg.ShowModal() == wxID_OK )
              m_model_frame->do_action
                ( new action_modify_mark
                  ( m, dlg.get_mark_name(), dlg.get_mark_animation(),
                    dlg.apply_angle_to_animation(),
                    dlg.pause_animation_when_hidden(),
                    dlg.reset_animation_with_action() ) );

            fill();
            m_mark_list->Select(index);
          }
      }
} // model_list_frame::on_show_properties()


/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a mark selection.
 * \param event This event occured.
 */
void bf::mark_list_frame::on_select_mark(wxCommandEvent& WXUNUSED(event))
{
  std::string s(wx_to_std_string(m_mark_list->GetStringSelection()));

  if ( !s.empty() )
    select_mark( s );
} // mark_list_frame::on_select_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when closing the window.
 * \param event This event occured.
 */
void bf::mark_list_frame::on_close(wxCloseEvent& event)
{
  if ( event.CanVeto() )
    {
      Hide();
      event.Veto();
    }
} // mark_list_frame::on_close()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::mark_list_frame, wxPanel)
  EVT_BUTTON( wxID_NEW, bf::mark_list_frame::on_new_mark )
  EVT_BUTTON( wxID_DELETE, bf::mark_list_frame::on_delete )
  EVT_BUTTON( wxID_PROPERTIES, bf::mark_list_frame::on_show_properties )
  EVT_LISTBOX_DCLICK( bf::mark_list_frame::IDC_MARK_LIST,
                      bf::mark_list_frame::on_show_properties )
  EVT_LISTBOX(bf::mark_list_frame::IDC_MARK_LIST,
              bf::mark_list_frame::on_select_mark )
  EVT_CLOSE( bf::mark_list_frame::on_close )
END_EVENT_TABLE()
