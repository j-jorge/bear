/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::main_frame class.
 * \author Julien Jorge
 */
#include "bf/main_frame.hpp"

#include "bf/about_dialog.hpp"
#include "bf/class_tree_ctrl.hpp"
#include "bf/config_frame.hpp"
#include "bf/gui_level.hpp"
#include "bf/image_pool.hpp"
#include "bf/ingame_view_frame.hpp"
#include "bf/item_class_selection_dialog.hpp"
#include "bf/item_instance.hpp"
#include "bf/layer_list_frame.hpp"
#include "bf/layer_properties_frame.hpp"
#include "bf/level_editor.hpp"
#include "bf/level_file_xml_reader.hpp"
#include "bf/level_properties_frame.hpp"
#include "bf/path_configuration.hpp"
#include "bf/properties_frame.hpp"
#include "bf/run_configuration_frame.hpp"
#include "bf/windows_layout.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/xml/run_configuration_file.hpp"

#include "bf/icon/item_class.xpm"

#include <wx/notebook.h>
#include <wx/filename.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>

#include <claw/logger.hpp>

DECLARE_APP(bf::level_editor)

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bf::main_frame::main_frame()
  : wxFrame(NULL, wxID_ANY, wxT("Bear Factory - Level editor")),
  m_windows_layout(NULL), m_tree_ctrl(NULL)
{
  load_run_configuration();

  SetSize( wxGetApp().get_config().main_rect );
  Show();

  SetSizer( new wxBoxSizer(wxVERTICAL) );

  wxNotebook* tabs =
    new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP );

  layer_list_frame* lay = new layer_list_frame( tabs );
  lay->Show( wxGetApp().get_config().layer_list_visible );
  tabs->AddPage( lay, _("Layers") );

  properties_frame* prop = new properties_frame( tabs );
  prop->Show( wxGetApp().get_config().properties_visible );
  tabs->AddPage( prop, _("Properties") );

  m_windows_layout =
    new windows_layout( *this, *prop, *lay );

  create_menu();
  create_controls();

  GetSizer()->Add( tabs, 1, wxALL | wxEXPAND );

  prop->set_window_layout( *m_windows_layout );

  turn_level_entries( false );
} // main_frame::main_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::main_frame::~main_frame()
{
  if (m_windows_layout != NULL)
    delete m_windows_layout;
} // main_frame::~main_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an empty level.
 * \param path The path of the level file in which it will be saved.
 */
void bf::main_frame::new_level( const wxString& path )
{
   std::string w = 
     path_configuration::get_instance().search_workspace
     ( wx_to_std_string(path) );
   
   if ( ! w.empty() )
     {
       workspace_environment env(w);
       level_properties_frame dlg(this, env);

       if ( dlg.ShowModal() == wxID_OK )
         {
           gui_level* lvl =
             new gui_level
             ( dlg.get_name(), dlg.get_width(), dlg.get_height(), 
               dlg.get_music() );

           add_level_view
             ( new ingame_view_frame(*m_windows_layout, lvl, env, path) );
         }
     }
} // main_frame::new_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a level.
 * \param path The path to the level to load.
 */
void bf::main_frame::load_level( const wxString& path )
{
  std::string w = 
     path_configuration::get_instance().search_workspace
     ( wx_to_std_string(path) );
   
   if ( ! w.empty() )
     {
       workspace_environment env(w);
       
       wxLogNull no_log;

       level_file_xml_reader reader;
       gui_level* lvl = reader.load(path, env);
       add_level_view
         ( new ingame_view_frame(*m_windows_layout, lvl, env, path) );
     }
} // main_frame::load_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the active level window.
 * \param view The active level window.
 */
void bf::main_frame::set_active_level( ingame_view_frame* view )
{
  turn_level_entries(view!=NULL);
  
  if ( view == NULL )
    m_tree_ctrl->unset_workspace();
  else
    m_tree_ctrl->set_workspace( view->get_workspace() );
} // main_frame::set_active_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the configuration to run to test a level.
 */
const bf::run_configuration& bf::main_frame::get_run_configuration() const
{
  return m_run_configuration;
} // main_frame::get_run_configuration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that a class is used.
 * \param c The class that is used.
 */
void bf::main_frame::use_class( const std::string& c )
{
  if ( m_tree_ctrl != NULL )
    m_tree_ctrl->use_class(c);
} // main_frame::use_class()

/*----------------------------------------------------------------------------*/
/**
 * Save the run configuration.
 */
void bf::main_frame::save_run_configuration() const
{
  // TO DO
  /*
  const std::string path =
    path_configuration::get_instance().get_config_directory() + "/run.xml";
  const xml::run_configuration_file writer = xml::run_configuration_file();
  std::ofstream output(path.c_str());
  
  writer.save( m_run_configuration, output );
  */
} // main_frame::save_run_configuration()

/*----------------------------------------------------------------------------*/
/**
 * Load the run configuration.
 */
void bf::main_frame::load_run_configuration()
{
  const std::string path =
    path_configuration::get_instance().get_config_directory() + "/run.xml";
  //  const xml::run_configuration_file reader = xml::run_configuration_file();
  const wxLogNull no_log;

  try
    {
      // TO DO
      //m_run_configuration =
      //  reader.load( wxGetApp().get_item_class_pool(), std_to_wx_string(path) );
    }
  catch( const std::exception& e )
    {
      claw::logger << claw::log_error << e.what() << std::endl;
    }
} // main_frame::load_run_configuration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Enable/disable the controls relative to the levels.
 * \param enable Tell if the controls are enabled.
 */
void bf::main_frame::turn_level_entries( bool enable )
{
  GetMenuBar()->Enable( wxID_SAVE, enable );
  GetMenuBar()->Enable( wxID_SAVEAS, enable );
  GetMenuBar()->Enable( ID_SAVE_ALL_LEVELS, enable );
  GetMenuBar()->Enable( ID_UPDATE_IMAGE_POOL, enable );

  GetToolBar()->EnableTool( wxID_SAVE, enable );
  GetToolBar()->ToggleTool(ID_NEW_ITEM, m_tree_frame->IsVisible());
} // main_frame::turn_level_entries()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the menu bar.
 */
void bf::main_frame::create_menu()
{
  wxMenuBar* menu_bar = new wxMenuBar();

  menu_bar->Append(create_level_menu(), _("&Level"));
  menu_bar->Append(create_help_menu(), _("&Help"));

  SetMenuBar(menu_bar);
} // main_frame::create_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
void bf::main_frame::create_controls()
{
  wxToolBar* bar = CreateToolBar();

  bar->AddTool
    ( wxID_NEW, _("&New"), wxArtProvider::GetBitmap(wxART_NEW), _("New") );
  bar->AddTool
    ( wxID_OPEN, _("&Open"), wxArtProvider::GetBitmap(wxART_FILE_OPEN),
      _("Open") );
  bar->AddTool
    ( wxID_SAVE, _("&Save"), wxArtProvider::GetBitmap(wxART_FILE_SAVE),
      _("Save") );
  bar->AddCheckTool
    ( ID_NEW_ITEM, _("&Class tree"), wxBitmap( item_class_xpm ),
      wxBitmap( item_class_xpm ), _("&Class tree") );

  bar->Realize();

  // class tree
  m_tree_frame = 
    new wxFrame( this, ID_SELECT_CLASS_DIALOG, _("Select an item class") );
  m_tree_ctrl =  new class_tree_ctrl( m_tree_frame, ID_TREE_FRAME );

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

  sizer->Add(m_tree_ctrl, 1, wxEXPAND);
  m_tree_frame->SetSizer(sizer);

  m_tree_frame->Connect
    ( ID_SELECT_CLASS_DIALOG, wxEVT_CLOSE_WINDOW,
      wxCloseEventHandler(main_frame::on_close_class_tree) );
} // main_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the "Level" menu of the menu bar.
 */
wxMenu* bf::main_frame::create_level_menu() const
{
  wxMenu* menu = new wxMenu();

  menu->Append( wxID_NEW, _("&New..."), _("Create a new level.") );
  menu->Append( wxID_OPEN, _("&Open..."), _("Open an existing level.") );
  menu->Append( wxID_SAVE, _("&Save"), _("Save the current level.") );
  menu->Append( wxID_SAVEAS, _("Save &as..."),
                _("Save the current level under a new name.") );
  menu->Append( ID_SAVE_ALL_LEVELS, _("Save a&ll"), _("Save all levels.") );
  menu->AppendSeparator();
  menu->Append( wxID_PREFERENCES, _("&Configuration..."),
                _("Configure the paths and programs.") );
  menu->Append( ID_RUN_CONFIGURATION, _("&Run configuration..."),
                _("Configure how the levels are executed.") );
  menu->Append( ID_UPDATE_IMAGE_POOL, _("&Update image pool"),
                _("Scan the directory of images for new images.") );
  menu->AppendSeparator();
  menu->Append( wxID_EXIT, _("&Exit"), _("Close everything and exit.") );

  return menu;
} // main_frame::create_level_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the "Help" menu of the menu bar.
 */
wxMenu* bf::main_frame::create_help_menu() const
{
  wxMenu* menu = new wxMenu();

  menu->Append( wxID_HELP, _("&Online documentation"),
                _("Open the online documentation in your web browser.") );

  menu->AppendSeparator();

  menu->Append( wxID_ABOUT, _("&About"),
                _("Display some informations about the data.") );

  return menu;
} // main_frame::create_help_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the configuration of the program.
 */
void bf::main_frame::save_config()
{
  wxGetApp().get_config().main_rect = GetScreenRect();

  wxGetApp().get_config().properties_visible =
    m_windows_layout->get_properties_frame().IsShown();
  wxGetApp().get_config().layer_list_visible =
    m_windows_layout->get_layer_list_frame().IsShown();

  if ( m_windows_layout->get_current_level_view() != NULL )    
    wxGetApp().get_config().default_level_window_rect =
      m_windows_layout->get_current_level_view()->GetScreenRect();

  wxGetApp().get_config().save();
} // main_frame::save_config()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an ingame_view_frame to the interface.
 * \param view The view to add.
 */
void bf::main_frame::add_level_view( ingame_view_frame* view )
{
  wxSize s;
  
  if ( m_windows_layout->get_current_level_view() != NULL )
    s = m_windows_layout->get_current_level_view()->GetSize();
  else
    {
      wxRect r = wxGetApp().get_config().default_level_window_rect;
      s = r.GetSize();
      view->SetPosition( r.GetPosition() );
    }

  view->SetSize(s);
  view->Show();
} // main_frame::add_level_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Configuration" menu.
 * \param event This event occured.
 */
void bf::main_frame::on_configuration_menu( wxCommandEvent& WXUNUSED(event) )
{
  wxGetApp().configure();
} // main_frame::on_configuration_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Run configuration" menu.
 * \param event This event occured.
 */
void
bf::main_frame::on_run_configuration_menu( wxCommandEvent& WXUNUSED(event) )
{
  run_configuration_frame dlg(this);
  dlg.set_configuration( m_run_configuration );

  if ( dlg.ShowModal() == wxID_OK )
    {
      // The run_configuration_frame does not allow to edit the items yet, so we
      // keep the old ones.
      run_configuration r = dlg.get_configuration();
      for ( std::size_t i=0; i!=m_run_configuration.get_items_count(); ++i )
        r.append_item( m_run_configuration.get_item(i) );

      m_run_configuration = r;
      save_run_configuration();
    }
} // main_frame::on_run_configuration_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Update image pool" menu.
 * \param event This event occured.
 */
void bf::main_frame::on_update_image_pool_menu
( wxCommandEvent& WXUNUSED(event) )
{

} // main_frame::on_update_image_pool_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Item properties" in the window menu.
 * \param event This event occured.
 */
void bf::main_frame::on_item_properties_menu( wxCommandEvent& event )
{
  m_windows_layout->get_properties_frame().Show( event.IsChecked() );
} // main_frame::on_item_properties_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Layer list" in the window menu.
 * \param event This event occured.
 */
void bf::main_frame::on_layer_list_menu( wxCommandEvent& event )
{
  m_windows_layout->get_layer_list_frame().Show( event.IsChecked() );
} // main_frame::on_layer_list_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "New level".
 * \param event This event occured.
 */
void bf::main_frame::on_new_level( wxCommandEvent& WXUNUSED(event) )
{
  new_level( wxEmptyString );
} // main_frame::on_new_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Open level".
 * \param event This event occured.
 */
void bf::main_frame::on_open_level( wxCommandEvent& WXUNUSED(event) )
{
  wxFileName path;

  if ( m_windows_layout->get_current_level_view() != NULL )
    {
      path = m_windows_layout->get_current_level_view()->get_level_file();

      for ( windows_layout::iterator it=m_windows_layout->begin();
            path.GetPath().empty() && (it!=m_windows_layout->end()); ++it )
        path = it->get_level_file();
    }

  wxFileDialog dlg( this, _("Choose a level"), path.GetPath(), wxEmptyString,
                    _("Level files (*.lvl)|*.lvl"),
                    wxFD_OPEN | wxFD_FILE_MUST_EXIST );

  if ( dlg.ShowModal() == wxID_OK )
    try
      {
        load_level( dlg.GetPath() );
      }
    catch( std::exception& e )
      {
        wxMessageDialog msg
          ( this, std_to_wx_string( e.what() ),
            _("Error in level loading."), wxOK );

        msg.ShowModal();
      }
} // main_frame::on_open_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Save as".
 * \param event This event occured.
 */
void bf::main_frame::on_save_as( wxCommandEvent& WXUNUSED(event) )
{
  m_windows_layout->get_current_level_view()->save_as();
} // main_frame::on_save_as()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Save".
 * \param event This event occured.
 */
void bf::main_frame::on_save( wxCommandEvent& WXUNUSED(event) )
{
  m_windows_layout->get_current_level_view()->save();
} // main_frame::on_save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Save all".
 * \param event This event occured.
 */
void bf::main_frame::on_save_all( wxCommandEvent& WXUNUSED(event) )
{
  windows_layout::iterator it;

  for ( it=m_windows_layout->begin(); it!=m_windows_layout->end(); ++it )
    it->save();
} // main_frame::on_save_all()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "New item".
 * \param event This event occured.
 */
void bf::main_frame::on_new_item( wxCommandEvent& event )
{
  m_tree_frame->Show( event.IsChecked() );
  GetToolBar()->ToggleTool(ID_NEW_ITEM, m_tree_frame->IsVisible());
} // main_frame::on_new_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Exit".
 * \param event This event occured.
 */
void bf::main_frame::on_exit( wxCommandEvent& WXUNUSED(event) )
{
  Close();
} // main_frame::on_exit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to the event that occurs when opening menus.
 * \param event This event occured.
 */
void bf::main_frame::on_menu_open( wxMenuEvent& WXUNUSED(event) )
{
  GetMenuBar()->Check
    ( ID_ITEM_PROPERTIES, m_windows_layout->get_properties_frame().IsShown() );

  GetMenuBar()->Check
    ( ID_LAYER_LIST, m_windows_layout->get_layer_list_frame().IsShown() );
} // main_frame::on_menu_open()

/*----------------------------------------------------------------------------*/
/**
 * \brief A menu entry is highlighted.
 * \param event This event occured.
 */
void bf::main_frame::on_menu_highlight( wxMenuEvent& event )
{
  ingame_view_frame* current_view = m_windows_layout->get_current_level_view();

  if ( current_view != NULL )
    current_view->GetStatusBar()->SetStatusText
      ( GetMenuBar()->GetHelpString(event.GetMenuId()), 0 );
} // main_frame::on_menu_highlight()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to an activation of the "online doc" menu.
 * \param event This event occured.
 */
void bf::main_frame::on_online_doc( wxCommandEvent& WXUNUSED(event) )
{
  wxLaunchDefaultBrowser
    ( wxT("http://plee-the-bear.sourceforge.net/" )
      wxT("wiki/index.php/%C9diteur_de_niveaux") );
} // main_frame::on_online_doc()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to an activation of the "about" menu.
 * \param event This event occured.
 */
void bf::main_frame::on_about( wxCommandEvent& WXUNUSED(event) )
{
  about_dialog dlg(*this);

  dlg.ShowModal();
} // main_frame::on_about()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when closing the window.
 * \param event This event occured.
 */
void bf::main_frame::on_close( wxCloseEvent& event )
{
  save_config();

  bool quit = !event.CanVeto();

  if ( !quit )
    {
      quit = true;

      bool changed = false;
      windows_layout::iterator it;

      for ( it=m_windows_layout->begin();
            !changed && it!=m_windows_layout->end(); ++it)
        changed = it->is_changed();

      if ( changed )
        {
          wxMessageDialog dlg
            ( this,
              _("Some levels are not saved."
                 " Do you want to save them now?"),
              _("Level is not saved."), wxYES_NO | wxCANCEL );

          int answer = dlg.ShowModal();

          if ( answer == wxID_CANCEL )
            quit = false;
          else if ( answer == wxID_YES )
            for ( it=m_windows_layout->begin();
                  quit && it!=m_windows_layout->end(); ++it)
              quit = it->save();
        }
    }

  if ( quit )
    event.Skip();
  else
    event.Veto();
} // main_frame::on_close()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when the user picks a class in the class list.
 * \param event This event occured.
 */
void bf::main_frame::on_class_selected( class_selected_event& event )
{
  ingame_view_frame* view = m_windows_layout->get_current_level_view();

  if ( view != NULL )
    view->add_item(event.get_class_name());
} // main_frame::on_class_selected()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when closing the class tree.
 * \param event This event occurred.
 */
void bf::main_frame::on_close_class_tree(wxCloseEvent& event)
{
  if ( event.CanVeto() )
    {
      Hide();
      event.Veto();
    }
  else
    Destroy();
} // main_frame::on_close_class_tree()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::main_frame, wxFrame)
  EVT_BUTTON( wxID_NEW, bf::main_frame::on_new_level )
  EVT_BUTTON( wxID_OPEN, bf::main_frame::on_open_level )
  EVT_BUTTON( wxID_SAVE, bf::main_frame::on_save )
  EVT_BUTTON( ID_NEW_ITEM, bf::main_frame::on_new_item )

  EVT_MENU( wxID_PREFERENCES, bf::main_frame::on_configuration_menu )
  EVT_MENU( bf::main_frame::ID_RUN_CONFIGURATION,
            bf::main_frame::on_run_configuration_menu )
  EVT_MENU( bf::main_frame::ID_UPDATE_IMAGE_POOL,
            bf::main_frame::on_update_image_pool_menu )
  EVT_MENU( bf::main_frame::ID_ITEM_PROPERTIES,
            bf::main_frame::on_item_properties_menu )
  EVT_MENU( bf::main_frame::ID_LAYER_LIST,
            bf::main_frame::on_layer_list_menu )

  EVT_MENU( wxID_NEW, bf::main_frame::on_new_level )
  EVT_MENU( wxID_OPEN, bf::main_frame::on_open_level )
  EVT_MENU( wxID_SAVE, bf::main_frame::on_save )
  EVT_MENU( wxID_SAVEAS, bf::main_frame::on_save_as )
  EVT_MENU( bf::main_frame::ID_SAVE_ALL_LEVELS, bf::main_frame::on_save_all )
  EVT_MENU( ID_NEW_ITEM, bf::main_frame::on_new_item )

  EVT_MENU( wxID_EXIT, bf::main_frame::on_exit )
  EVT_MENU_OPEN( bf::main_frame::on_menu_open )
  EVT_MENU_HIGHLIGHT_ALL( bf::main_frame::on_menu_highlight )

  EVT_MENU( wxID_HELP, bf::main_frame::on_online_doc )
  EVT_MENU( wxID_ABOUT, bf::main_frame::on_about )

  EVT_CLOSE( bf::main_frame::on_close )

  EVT_CLASS_SELECTED( bf::main_frame::ID_TREE_FRAME,
                      bf::main_frame::on_class_selected )
END_EVENT_TABLE()
