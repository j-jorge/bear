/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::sound_frame class.
 * \author Sébastien Angibaud
 */
#include "bf/sound_frame.hpp"

#include "bf/custom_type.hpp"
#include "bf/value_editor_dialog.hpp"
#include "bf/base_file_edit.hpp"

#include "bf/wx_facilities.hpp"

/*----------------------------------------------------------------------------*/
const wxEventType bf::sound_description_event::change_event_type =
  wxNewEventType();

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
bf::sound_description_event::sound_description_event
( wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id)
{

} // sound_description_event::sound_description_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
wxEvent* bf::sound_description_event::Clone() const
{
  return new sound_description_event(*this);
} // sound_description_event::Clone()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The frame owning this one.
 * \param id The identifier of this window.
 */
bf::sound_frame::sound_frame( wxWindow* parent, wxWindowID id )
  : wxPanel( parent, id ), m_sound_files(NULL), m_play_globally(NULL)
{
  create_controls();
} // sound_frame::sound_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fills the control with the properties of a given sound description.
 * \param sound The sound description.
 */
void bf::sound_frame::set_sound_description( sound_description d )
{
  m_sound_files->Clear();

  const sound_description::sound_set sounds( d.get_files() );

  for ( sound_description::sound_set::const_iterator it=sounds.begin();
        it!=sounds.end(); ++it )
    m_sound_files->Append( std_to_wx_string(*it) );

  m_play_globally->SetValue( d.is_played_globally() );
} // sound_frame::set_model_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the sound description corresponding to the content of the
 *        controls.
 */
bf::sound_description bf::sound_frame::get_sound_description() const
{
  sound_description result;

  result.set_played_globally( m_play_globally->GetValue() );

  for ( std::size_t i=0; i!=m_sound_files->GetCount(); ++i )
    result.insert_file( wx_to_std_string( m_sound_files->GetString( i ) ) );

  return result;
} // sound_frame::get_sound_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::sound_frame::create_controls()
{
  create_member_controls();
  create_sizer_controls();
} // sound_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls for which we keep a reference.
 */
void bf::sound_frame::create_member_controls()
{
  m_sound_files = new wxListBox( this, wxID_ANY );
  m_play_globally =
    new wxCheckBox( this, ID_PLAY_GLOBALLY, _("Play globally") );
} // sound_frame::create_member_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::sound_frame::create_sizer_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

  // the sizer for the buttons
  wxBoxSizer* s_sizer = new wxBoxSizer( wxVERTICAL );

  s_sizer->Add( new wxButton( this, wxID_NEW ), 0, wxEXPAND | wxALL, 0 );
  s_sizer->Add( new wxButton( this, wxID_EDIT ), 0, wxEXPAND | wxALL, 0 );
  s_sizer->Add( new wxButton( this, wxID_DELETE ), 0, wxEXPAND | wxALL, 0 );

  sizer->Add( m_sound_files, 1, wxEXPAND );
  sizer->Add( s_sizer, 0, wxEXPAND );

  s_sizer = new wxBoxSizer( wxVERTICAL );
  s_sizer->Add( m_play_globally, 0, wxEXPAND | wxALL, 0 );
  s_sizer->Add( sizer, 1, wxEXPAND | wxALL, 0 );

  SetSizer(s_sizer);
} // sound_frame::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Dispatch an event telling that the sound description has changed.
 */
void bf::sound_frame::dispatch_change_event()
{
  sound_description_event event
    ( sound_description_event::change_event_type, GetId() );
  event.SetEventObject(this);
  ProcessEvent(event);
} // sound_frame::dispatch_change_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds a new sound file in the list.
 * \param event This event occured.
 */
void bf::sound_frame::on_new_sound( wxCommandEvent& WXUNUSED(event) )
{
  typedef custom_type<std::string> custom_string;
  typedef value_editor_dialog< base_file_edit<custom_string> > dialog_type;

  dialog_type dialog( *this, _("Pick a sound file"), custom_string() );
  dialog.get_editor().set_filter( _("Sound files|*.ogg") );

  if ( dialog.ShowModal() == wxID_OK )
    {
      const wxString path( std_to_wx_string( dialog.get_value().get_value() ) );

      if ( !path.IsEmpty()
           && (m_sound_files->FindString( path ) == wxNOT_FOUND) )
        {
          m_sound_files->Append( path );
          dispatch_change_event();
        }
    }
} // sound_frame::on_new_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Edits the selected sound.
 * \param event This event occured.
 */
void bf::sound_frame::on_edit_sound( wxCommandEvent& WXUNUSED(event) )
{
  const int index( m_sound_files->GetSelection() );

  if ( index == wxNOT_FOUND )
    return;
  
  typedef custom_type<std::string> custom_string;
  typedef value_editor_dialog< base_file_edit<custom_string> > dialog_type;

  const custom_string prev
    ( wx_to_std_string( m_sound_files->GetString( index ) ) );
  dialog_type dialog( *this, _("Pick a sound file"), prev );
  dialog.get_editor().set_filter( _("Sound files|*.ogg") );

  if ( dialog.ShowModal() == wxID_OK )
    {
      const wxString path( std_to_wx_string( dialog.get_value().get_value() ) );

      if ( !path.IsEmpty()
           && (m_sound_files->FindString( path ) == wxNOT_FOUND) )
        {
          m_sound_files->SetString( index, path );
          dispatch_change_event();
        }
    }
} // sound_frame::on_edit_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Erase the selected strings.
 * \param event This event occured.
 */
void bf::sound_frame::on_remove_sound( wxCommandEvent& WXUNUSED(event) )
{
  wxArrayInt selected_index;
  m_sound_files->GetSelections( selected_index );

  int i( selected_index.GetCount() );

  if ( i == 0 )
    return;

  while ( i != 0 )
    {
      --i;
      m_sound_files->Delete( selected_index[i] );
    }

  dispatch_change_event();
} // sound_frame::on_remove_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Notifies about the change of the "play globally" property.
 * \param event This event occured.
 */
void bf::sound_frame::on_play_globally_change( wxCommandEvent& WXUNUSED(event) )
{
  dispatch_change_event();
} // sound_frame::on_play_globally_change()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::sound_frame, wxPanel)
  EVT_BUTTON( wxID_NEW, bf::sound_frame::on_new_sound )
  EVT_BUTTON( wxID_DELETE, bf::sound_frame::on_remove_sound )
  EVT_BUTTON( wxID_EDIT, bf::sound_frame::on_edit_sound )

  EVT_CHECKBOX( bf::sound_frame::ID_PLAY_GLOBALLY,
                bf::sound_frame::on_play_globally_change )
END_EVENT_TABLE()
