/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::config_frame class.
 * \author Julien Jorge
 */
#include "bf/config_frame.hpp"

#include "bf/path_configuration.hpp"
#include "bf/wx_facilities.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this dialog.
 */
bf::config_frame::config_frame( wxWindow* parent )
  : wxDialog( parent, wxID_ANY, _("Configuration"), wxDefaultPosition, 
             wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER )
{
  create_controls();
  Fit();
} // config_frame::config_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the controls with the values of the config.
 */
void bf::config_frame::fill_controls()
{
  m_item_classes_list->Clear();
  m_data_path_list->Clear();
  m_workspaces->Clear();

  std::set< std::string > workspaces;
  path_configuration::get_instance().get_workspace_names( workspaces );
  std::set< std::string >::const_iterator it_set;

  for ( it_set = workspaces.begin(); it_set != workspaces.end(); ++it_set )
    {
      m_workspaces->Append( std_to_wx_string(*it_set) );
  
      std::list<std::string>::const_iterator it;
      std::map<std::string, std::list<std::string> >::const_iterator it_map;
          
      it_map = path_configuration::get_instance().item_class_path.find(*it_set);
      if ( it_map != path_configuration::get_instance().item_class_path.end() )
        for ( it = it_map->second.begin(); it != it_map->second.end(); ++it )
          m_item_class_path[*it_set].push_back(*it);
      
      it_map = path_configuration::get_instance().data_path.find(*it_set);
      if ( it_map != path_configuration::get_instance().data_path.end() )
        for ( it = it_map->second.begin(); it != it_map->second.end(); ++it )
          m_data_path[*it_set].push_back(*it);
    }

  if  ( ! workspaces.empty() )
    {
      m_workspaces->SetSelection(0);      
      fill_list_view();
    }
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the list_view.
 */
void bf::config_frame::fill_list_view()
{
  m_item_classes_list->Clear();
  m_data_path_list->Clear();
  
  if ( ! m_workspaces->IsEmpty() )
    {
      std::string s = wx_to_std_string( m_workspaces->GetStringSelection() );
      std::list<std::string>::const_iterator it;
      
      for ( it = m_item_class_path[s].begin();
            it != m_item_class_path[s].end(); ++it )
        m_item_classes_list->Append( std_to_wx_string(*it) );
      
      for ( it = m_data_path[s].begin(); it != m_data_path[s].end(); ++it )
        m_data_path_list->Append( std_to_wx_string(*it) );
    }
} // config_frame::fill_list_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of our frame.
 */
void bf::config_frame::create_controls()
{
  create_member_controls();
  create_sizer_controls();
  fill_controls();
} // config_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls for which we keep a reference.
 */
void bf::config_frame::create_member_controls()
{
  m_workspaces = new wxChoice( this, IDC_CHOICE );
  m_item_classes_list = new wxListBox( this, wxID_ANY );
  m_data_path_list = new wxListBox( this, wxID_ANY );
} // config_frame::create_member_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::config_frame::create_sizer_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );
  sizer->Add( m_workspaces, 0, wxEXPAND );

  // item classes
  wxBoxSizer* v_sizer = new wxBoxSizer( wxVERTICAL );
  wxStaticBoxSizer* s_sizer = new wxStaticBoxSizer
    ( wxHORIZONTAL, this, _("Path to the item class files") );

  v_sizer->Add
    ( new wxButton( this, IDC_BROWSE_ITEM_CLASSES_BUTTON, wxT("+"),
                    wxDefaultPosition, wxSize(30, -1) ), 0, wxALL, 5 );
  v_sizer->Add
    ( new wxButton( this, IDC_ERASE_ITEM_CLASSES_BUTTON, wxT("-"),
                    wxDefaultPosition, wxSize(30, -1) ), 0, wxALL, 5 );

  s_sizer->Add( m_item_classes_list, 1, wxEXPAND | wxALL, 5 );
  s_sizer->Add( v_sizer, 0, wxEXPAND );
  sizer->Add( s_sizer, 0, wxEXPAND );

  // data path
  v_sizer = new wxBoxSizer( wxVERTICAL );
  s_sizer = new wxStaticBoxSizer
    ( wxHORIZONTAL, this, _("Path to data directory of the game") );

  v_sizer->Add
    ( new wxButton( this, IDC_BROWSE_DATA_PATH_BUTTON, wxT("+"),
                    wxDefaultPosition, wxSize(30, -1) ), 0, wxALL, 5 );
  v_sizer->Add
    ( new wxButton( this, IDC_ERASE_DATA_PATH_BUTTON, wxT("-"),
                    wxDefaultPosition, wxSize(30, -1) ), 0, wxALL, 5 );

  s_sizer->Add( m_data_path_list, 1, wxEXPAND | wxALL, 5 );
  s_sizer->Add( v_sizer, 0, wxEXPAND );
  sizer->Add( s_sizer, 0, wxEXPAND );

  // ok / cancel
  sizer->Add( CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0,
              wxALL | wxCENTER, 5 );

  SetSizer(sizer);
} // config_frame::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "OK" button.
 * \param event This event occured.
 */
void bf::config_frame::on_ok( wxCommandEvent& WXUNUSED(event) )
{
  path_configuration::get_instance().item_class_path.clear();
  path_configuration::get_instance().data_path.clear();

  path_configuration::get_instance().set_item_class_path( m_item_class_path );
  path_configuration::get_instance().set_data_path( m_data_path );

  path_configuration::get_instance().save();

  EndModal(wxID_OK);
} // config_frame::on_ok()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "Cancel" button.
 * \param event This event occured.
 */
void bf::config_frame::on_cancel( wxCommandEvent& WXUNUSED(event) )
{
  fill_controls();

  EndModal(wxID_CANCEL);
} // config_frame::on_cancel()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "Browse" button for the item files.
 * \param event This event occured.
 */
void bf::config_frame::on_browse_item_classes( wxCommandEvent& WXUNUSED(event) )
{
  wxDirDialog diag(this);

  if ( diag.ShowModal() == wxID_OK )
    {
      if ( m_workspaces->GetSelection() != wxNOT_FOUND )
        {
          m_item_classes_list->Append( diag.GetPath() );
          std::string s = 
            wx_to_std_string( m_workspaces->GetStringSelection() );
          m_item_class_path[s].push_back( wx_to_std_string(diag.GetPath()) );
        }
    }
} // config_frame::on_browse_item_classes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "Erase" button for the item files.
 * \param event This event occured.
 */
void bf::config_frame::on_erase_item_classes( wxCommandEvent& WXUNUSED(event) )
{
  int i = m_item_classes_list->GetSelection();

  if ( i != wxNOT_FOUND && m_workspaces->GetSelection() != wxNOT_FOUND )
    {
      m_item_classes_list->Delete(i);
      
      std::string s = 
        wx_to_std_string( m_workspaces->GetStringSelection() );
      m_item_class_path[s].clear();

      for ( unsigned int j = 0; j < m_item_classes_list->GetCount(); ++j )
        m_item_class_path[s].push_back
          ( wx_to_std_string( m_item_classes_list->GetString(j) ) );
    }
} // config_frame::on_erase_item_classes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "Browse" button for the data directory.
 * \param event This event occured.
 */
void bf::config_frame::on_browse_data_path( wxCommandEvent& WXUNUSED(event) )
{
  wxDirDialog diag(this);

  if ( diag.ShowModal() == wxID_OK )
    {
      if ( m_workspaces->GetSelection() != wxNOT_FOUND )
        {
          m_data_path_list->Append( diag.GetPath() );
          std::string s = 
            wx_to_std_string( m_workspaces->GetStringSelection() );
          m_data_path[s].push_back( wx_to_std_string(diag.GetPath()) );
        }
    }
} // config_frame::on_browse_data_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "Erase" button for the data directory.
 * \param event This event occured.
 */
void bf::config_frame::on_erase_data_path( wxCommandEvent& WXUNUSED(event) )
{
  int i = m_data_path_list->GetSelection();

  if ( i != wxNOT_FOUND && m_workspaces->GetSelection() != wxNOT_FOUND )
    {
      m_data_path_list->Delete(i);
      
      std::string s = 
        wx_to_std_string( m_workspaces->GetStringSelection() );
      m_data_path[s].clear();

      for ( unsigned int j = 0; j < m_data_path_list->GetCount(); ++j )
        m_data_path[s].push_back
          ( wx_to_std_string( m_data_path_list->GetString(j) ) );
    }
} // config_frame::on_erase_data_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a selection of workspace.
 * \param event This event occured.
 */
void bf::config_frame::on_select_workspace( wxCommandEvent& WXUNUSED(event) )
{
  std::cout << "SELECT" << std::endl;
  fill_list_view();
} // config_frame::on_select_workspace()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::config_frame, wxDialog)
  EVT_BUTTON( wxID_OK, bf::config_frame::on_ok )
  EVT_BUTTON( wxID_CANCEL, bf::config_frame::on_cancel )
  EVT_BUTTON( bf::config_frame::IDC_BROWSE_ITEM_CLASSES_BUTTON,
              bf::config_frame::on_browse_item_classes )
  EVT_BUTTON( bf::config_frame::IDC_ERASE_ITEM_CLASSES_BUTTON,
              bf::config_frame::on_erase_item_classes )
  EVT_BUTTON( bf::config_frame::IDC_BROWSE_DATA_PATH_BUTTON,
              bf::config_frame::on_browse_data_path )
  EVT_BUTTON( bf::config_frame::IDC_ERASE_DATA_PATH_BUTTON,
              bf::config_frame::on_erase_data_path )
  EVT_CHOICE( IDC_CHOICE, bf::config_frame::on_select_workspace )
END_EVENT_TABLE()
