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
#include "bf/config_frame.hpp"
#include "bf/gui_model.hpp"
#include "bf/image_pool.hpp"
#include "bf/model_editor.hpp"
#include "bf/model_frame.hpp"
#include "bf/model_properties_ctrl.hpp"
#include "bf/path_configuration.hpp"
#include "bf/windows_layout.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/xml/model_file.hpp"

#include <fstream>
#include <wx/filename.h>
#include <wx/msgdlg.h>

DECLARE_APP(bf::model_editor)

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bf::main_frame::main_frame()
  : wxFrame(NULL, wxID_ANY, wxT("Bear Factory - Model editor")),
  m_model_properties(NULL), m_windows_layout(NULL)
{
  SetSize( wxGetApp().get_config().main_rect );

  create_menu();
  create_controls();
  turn_model_entries(false);

  m_windows_layout = new windows_layout(*this);

  Show();
} // main_frame::main_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::main_frame::~main_frame()
{
  if (m_windows_layout != NULL)
    delete m_windows_layout;
} // main_frame::main_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Enable/disable the controls relative to the models.
 * \param enable Tell if the controls are enabled.
 */
void bf::main_frame::turn_model_entries( bool enable )
{
  GetMenuBar()->Enable( ID_SAVE_ALL_MODELS, enable );
  GetMenuBar()->Enable( ID_UPDATE_IMAGE_POOL, enable );
} // main_frame::turn_model_entries()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a new model.
 * \param path The path of the model file in which it will be saved.
 */
void bf::main_frame::new_model( const wxString& path )
{
  gui_model* mdl = new gui_model();
  add_model_view( new model_frame(*m_windows_layout, mdl, path) );
} // main_frame::new_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a model. Return true if the model has been loaded.
 * \param path The path to the model to load.
 */
bool bf::main_frame::load_model( const wxString& path )
{
  bool result = false;
  wxLogNull no_log;

  gui_model* mdl(NULL);

  try
    {
      std::string w = 
        path_configuration::get_instance().search_workspace
        ( wx_to_std_string(path) );
      
      if ( ! w.empty() )
        {
          workspace_environment env(w);
      
          xml::model_file reader;
          mdl = reader.load(path, &env);

          add_model_view( new model_frame(*m_windows_layout, mdl, path) );
          result = true;
        }
      else
        std::cout << "Error. No workspace is available for model " 
                  << wx_to_std_string( path ) << std::endl;
    }
  catch( std::exception& e )
    {
      delete mdl;
      throw;
    }

  return result;
} // main_frame::load_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell which model frame is active.
 * \param m The active model frame.
 */
void bf::main_frame::set_active_model( model_frame* m )
{
  m_model_properties->set_model_frame( m );

  if ( m_windows_layout->get_current_model_frame() == NULL )
    turn_model_entries(false);
} // main_frame::make_title()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update windows when the current action is modified.
 */
void bf::main_frame::update_action()
{
  m_model_properties->update_action();
} // main_frame::update_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update windows when the current snapshot is modified.
 */
void bf::main_frame::update_snapshot()
{
  m_model_properties->update_snapshot();
} // main_frame::update_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update windows when the current mark is modified.
 */
void bf::main_frame::update_mark()
{
  m_model_properties->update_mark();
} // main_frame::update_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the menu bar.
 */
void bf::main_frame::create_menu()
{
  wxMenuBar* menu_bar = new wxMenuBar();

  menu_bar->Append(create_model_menu(), _("&Model"));
  menu_bar->Append(create_help_menu(), _("&Help"));

  SetMenuBar(menu_bar);
} // main_frame::create_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
void bf::main_frame::create_controls()
{
  m_model_properties = new model_properties_ctrl(this);
} // main_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the "Model" menu of the menu bar.
 */
wxMenu* bf::main_frame::create_model_menu() const
{
  wxMenu* menu = new wxMenu();

  menu->Append( wxID_NEW, _("&New..."), _("Create a new model.") );
  menu->Append( wxID_OPEN, _("&Open..."), _("Open an existing model.") );
  menu->Append( ID_SAVE_ALL_MODELS, _("&Save all"), _("Save all models.") );
  menu->AppendSeparator();
  menu->Append( wxID_PREFERENCES, _("&Configuration..."),
                _("Configure the paths and programs.") );
  menu->Append( ID_UPDATE_IMAGE_POOL, _("&Update image pool"),
                _("Scan the directory of images for new images.") );
  menu->AppendSeparator();
  menu->Append( wxID_EXIT, _("&Exit"), _("Close everything and exit.") );

  return menu;
} // main_frame::create_model_menu()

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

  if ( m_windows_layout->get_current_model_frame() != NULL )    
    wxGetApp().get_config().default_model_window_rect =
      m_windows_layout->get_current_model_frame()->GetScreenRect();

  wxGetApp().get_config().save();
} // main_frame::save_config()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an ingame_view_frame to the interface.
 * \param m The frame to add.
 */
void bf::main_frame::add_model_view( model_frame* m )
{
  wxSize s;

  if ( m_windows_layout->get_current_model_frame() != NULL )
    s = m_windows_layout->get_current_model_frame()->GetSize();
  else
    {
      wxRect r = wxGetApp().get_config().default_model_window_rect;
      s = r.GetSize();
      m->SetPosition( r.GetPosition() );
    }

  set_active_model(m);
  m->SetSize(s);
  m->Show();
  turn_model_entries(true);
} // main_frame::add_model_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Configuration" menu.
 * \param event This event occured.
 */
void bf::main_frame::on_configuration_menu(wxCommandEvent& WXUNUSED(event))
{
  wxGetApp().configure();
} // main_frame::on_configuration_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Update image pool" menu.
 * \param event This event occured.
 */
void bf::main_frame::on_update_image_pool_menu(wxCommandEvent& WXUNUSED(event))
{
  // to do
} // main_frame::on_update_image_pool_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "New model".
 * \param event This event occured.
 */
void bf::main_frame::on_new_model(wxCommandEvent& WXUNUSED(event))
{
  new_model( wxEmptyString );
} // main_frame::on_new_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Open model".
 * \param event This event occured.
 */
void bf::main_frame::on_open_model(wxCommandEvent& WXUNUSED(event))
{
  wxFileName path;

  if ( m_windows_layout->get_current_model_frame() != NULL )
    {
      path = m_windows_layout->get_current_model_frame()->get_model_file();

      for ( windows_layout::iterator it=m_windows_layout->begin();
            path.GetPath().empty() && (it!=m_windows_layout->end()); ++it )
        path = it->get_model_file();
    }

  wxFileDialog dlg( this, _("Choose a model"), path.GetPath(), wxEmptyString,
                    _("Model files (*.mdl)|*.mdl"),
                    wxFD_OPEN | wxFD_FILE_MUST_EXIST );

  if ( dlg.ShowModal() == wxID_OK )
    load_model( dlg.GetPath() );
} // main_frame::on_open_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Save all".
 * \param event This event occured.
 */
void bf::main_frame::on_save_all(wxCommandEvent& WXUNUSED(event))
{
  windows_layout::iterator it;

  for ( it=m_windows_layout->begin(); it!=m_windows_layout->end(); ++it )
    it->save();
} // main_frame::on_save_all()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Exit".
 * \param event This event occured.
 */
void bf::main_frame::on_exit(wxCommandEvent& WXUNUSED(event))
{
  Close();
} // main_frame::on_exit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to an activation of the "online doc" menu.
 * \param event This event occured.
 */
void bf::main_frame::on_online_doc(wxCommandEvent& WXUNUSED(event))
{
  wxLaunchDefaultBrowser
    ( wxT("http://plee-the-bear.sourceforge.net/" )
      wxT("wiki/index.php/%C9diteur_de_mod%E8les") );
} // main_frame::on_online_doc()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to an activation of the "about" menu.
 * \param event This event occured.
 */
void bf::main_frame::on_about(wxCommandEvent& WXUNUSED(event))
{
  about_dialog dlg(*this);

  dlg.ShowModal();
} // main_frame::on_about()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when closing the window.
 * \param event This event occured.
 */
void bf::main_frame::on_close(wxCloseEvent& event)
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
              _("Some models are not saved."
                 " Do you want to save them now?"),
              _("Model is not saved."), wxYES_NO | wxCANCEL );

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
BEGIN_EVENT_TABLE(bf::main_frame, wxFrame)
  EVT_MENU( wxID_PREFERENCES, bf::main_frame::on_configuration_menu )
  EVT_MENU( bf::main_frame::ID_UPDATE_IMAGE_POOL,
            bf::main_frame::on_update_image_pool_menu )

  EVT_MENU( wxID_NEW, bf::main_frame::on_new_model )
  EVT_MENU( wxID_OPEN, bf::main_frame::on_open_model )
  EVT_MENU( bf::main_frame::ID_SAVE_ALL_MODELS, bf::main_frame::on_save_all )

  EVT_MENU( wxID_EXIT, bf::main_frame::on_exit )

  EVT_MENU( wxID_HELP, bf::main_frame::on_online_doc )
  EVT_MENU( wxID_ABOUT, bf::main_frame::on_about )

  EVT_CLOSE( bf::main_frame::on_close )
END_EVENT_TABLE()
