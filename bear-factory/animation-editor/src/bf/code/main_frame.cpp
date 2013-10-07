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

#include "bf/animation_editor.hpp"
#include "bf/about_dialog.hpp"
#include "bf/frame_duration_dialog.hpp"
#include "bf/image_pool.hpp"
#include "bf/animation_file_xml_reader.hpp"
#include "bf/animation_file_xml_writer.hpp"
#include "bf/path_configuration.hpp"
#include "bf/workspace.hpp"
#include "bf/wx_facilities.hpp"

#include "bf/icon/compile.xpm"

#include <fstream>
#include <wx/filename.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>

DECLARE_APP(bf::animation_editor)

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 * \param w The workspace.
 */
bf::main_frame::main_frame( const std::string & w )
: wxFrame(NULL, wxID_ANY, wxT("Bear Factory - Animation editor")),
  m_workspace(w)
{
  create_menu();
  create_toolbar();
  create_controls();
  Fit();
} // main_frame::main_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an animation.
 * \param path The path to the animation to load.
 */
void bf::main_frame::load_animation( const wxString& path )
{
  wxLogNull no_log;
  wxXmlDocument doc;
  animation_file_xml_reader reader;
  animation anim;

  if ( doc.Load(path) )
    anim = reader.load( doc.GetRoot(), &m_workspace );

  m_animation_edit->set_value(anim);
  m_last_saved_animation = m_animation_edit->get_value();
  m_animation_file = path;
  make_title();
} // main_frame::load_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Make the title of the window.
 */
void bf::main_frame::make_title()
{
  if ( m_animation_file.empty() )
    SetTitle( wxT("Bear Factory - Animation editor") );
  else
    SetTitle( wxFileName(m_animation_file).GetName() );

  if ( is_changed() )
    SetTitle( GetTitle() + wxT("*") );
} // main_frame::make_title()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the animation has changes that are not saved.
 */
bool bf::main_frame::is_changed() const
{
  return m_last_saved_animation != m_animation_edit->get_value();
} // main_frame::is_changed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the animation.
 */
bool bf::main_frame::save()
{
  bool result = false;

  if ( m_animation_file.empty() )
    result = save_as();
  else
    result = effective_save();

  return result;
} // main_frame::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the animation and ask the file to the user.
 */
bool bf::main_frame::save_as()
{
  bool result = false;
  wxFileDialog dlg( this, _("Choose a file"), wxEmptyString, m_animation_file,
                    _("Animation files (*.anim)|*.anim"),
                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT );

  if ( dlg.ShowModal() == wxID_OK )
    {
      m_animation_file = dlg.GetPath();
      result = effective_save();
    }

  return result;
} // main_frame::save_as()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the animation without asking anything.
 */
bool bf::main_frame::effective_save()
{
  bool result = m_animation_edit->validate();

  if (result)
    {
      std::string std_path( wx_to_std_string(m_animation_file) );
      std::ofstream f( std_path.c_str() );

      if (f)
        {
          animation_file_xml_writer writer;

          writer.save( f, m_animation_edit->get_value() );
          m_last_saved_animation = m_animation_edit->get_value();
          result = true;
          make_title();
        }
      else
        {
          wxMessageDialog dlg
            ( this, _("Error"), _("Can't open the animation file."), wxOK );

          dlg.ShowModal();
        }
    }

  return result;
} // main_frame::effective_save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Enable/disable the controls relative to the animation.
 * \param b Tell if the controls are enabled.
 */
void bf::main_frame::turn_animation_menu_entries( bool b )
{
  GetMenuBar()->Enable( wxID_SAVE, b );
  GetMenuBar()->Enable( wxID_SAVEAS, b );
} // main_frame::turn_animation_menu_entries()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the menu bar.
 */
void bf::main_frame::create_menu()
{
  wxMenuBar* menu_bar = new wxMenuBar();

  menu_bar->Append(create_animation_menu(), _("&Animation"));
  menu_bar->Append(create_edit_menu(), _("&Edit"));
  menu_bar->Append(create_help_menu(), _("&Help"));

  SetMenuBar(menu_bar);
} // main_frame::create_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the toolbar.
 */
void bf::main_frame::create_toolbar()
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
  bar->AddSeparator();
  bar->AddTool
    ( ID_COMPILE, _("&Compile"), wxBitmap(compile_xpm), _("Compile") );

  bar->Realize();
} // main_frame::create_toolbar()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
void bf::main_frame::create_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
  m_animation_edit = new animation_edit( *this, &m_workspace );

  sizer->Add(m_animation_edit, 1, wxEXPAND | wxALL, 5 );

  SetSizer(sizer);

  SetStatusBar( new wxStatusBar(this, wxID_ANY) );
} // main_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the "Edit" menu of the menu bar.
 */
wxMenu* bf::main_frame::create_edit_menu() const
{
  wxMenu* menu = new wxMenu();

  menu->Append
    ( ID_CHANGE_DURATION, _("&Change all durations..."),
      _("Change the duration of all frames at once.") );

  return menu;
} // main_frame::create_edit_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the "Animation" menu of the menu bar.
 */
wxMenu* bf::main_frame::create_animation_menu() const
{
  wxMenu* menu = new wxMenu();

  menu->Append( wxID_NEW, _("&New..."), _("Create a new animation.") );
  menu->Append( wxID_OPEN, _("&Open..."), _("Open an existing animation.") );
  menu->Append( wxID_SAVE, _("&Save"), _("Save the animation.") );
  menu->Append( wxID_SAVEAS, _("Save &as..."),
                _("Save the animation under a new name.") );
  menu->AppendSeparator();
  menu->Append( ID_COMPILE, _("&Compile\tF9"), _("Compile the animation.") );
  menu->AppendSeparator();
  menu->Append( wxID_PREFERENCES, _("&Configuration..."),
                _("Configure the paths of the datas.") );
  menu->Append( ID_UPDATE_IMAGE_POOL, _("&Update image pool"),
                _("Scan the directory of images for new images.") );
  menu->AppendSeparator();
  menu->Append( wxID_EXIT, _("&Exit"), _("Close the window.") );

  return menu;
} // main_frame::create_animation_menu()

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
                _("Display some informations about the program.") );

  return menu;
} // main_frame::create_help_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the configuration of the program.
 */
void bf::main_frame::save_config()
{
  wxGetApp().set_main_rect(GetScreenRect());
} // main_frame::save_config()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the animation.
 */
void bf::main_frame::compile_animation()
{
  Disable();
  Refresh();

  if ( m_animation_edit->validate() )
    {
      bool do_it = true;

      if ( m_animation_file.IsEmpty() )
        do_it = save_as();

      if ( do_it )
        compile_animation_no_check();
    }

  Enable();
  GetStatusBar()->SetStatusText( _("Compilation done."), 0 );
} // main_frame::compile_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the animation without checking anything.
 */
void bf::main_frame::compile_animation_no_check()
{
  try
    {
      wxGetApp().compile_animation
        ( m_animation_edit->get_value(), m_animation_file );
    }
  catch ( std::exception& e )
    {
      wxMessageDialog dlg
        ( this, _("Error"), std_to_wx_string(e.what()), wxOK );

      dlg.ShowModal();
    }
} // main_frame::compile_animation_no_check()

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
 * \brief Answer to a click on "Update image pool" menu.
 * \param event This event occured.
 */
void bf::main_frame::on_update_image_pool_menu
( wxCommandEvent& WXUNUSED(event) )
{
  // TO DO
} // main_frame::on_update_image_pool_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "New animation".
 * \param event This event occured.
 */
void bf::main_frame::on_new_animation( wxCommandEvent& WXUNUSED(event) )
{
  main_frame* frame = new main_frame(m_workspace.name);
  frame->Show();
} // main_frame::on_new_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Open animation".
 * \param event This event occured.
 */
void bf::main_frame::on_open_animation( wxCommandEvent& WXUNUSED(event) )
{
  const wxFileName path(m_animation_file);

  wxFileDialog dlg
    ( this, _("Choose an animation"), path.GetPath(), wxEmptyString,
      _("Animation files (*.anim)|*.anim"), wxFD_OPEN | wxFD_FILE_MUST_EXIST );

  if ( dlg.ShowModal() == wxID_OK )
    {
      if ( is_changed() || !m_animation_file.empty() )
        {
          main_frame* frm = new main_frame(m_workspace.name);
          frm->load_animation( dlg.GetPath() );
          frm->Show();
        }
      else
        load_animation( dlg.GetPath() );
    }
} // main_frame::on_open_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Save".
 * \param event This event occured.
 */
void bf::main_frame::on_save( wxCommandEvent& WXUNUSED(event) )
{
  save();
} // main_frame::on_save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Save as".
 * \param event This event occured.
 */
void bf::main_frame::on_save_as( wxCommandEvent& WXUNUSED(event) )
{
  save_as();
} // main_frame::on_save_as()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Compile".
 * \param event This event occured.
 */
void bf::main_frame::on_compile( wxCommandEvent& WXUNUSED(event) )
{
  compile_animation();
} // main_frame::on_compile()

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
 * \brief Answer to a click on the menu to change the duration of the frames.
 * \param event This event occured.
 */
void bf::main_frame::on_change_duration( wxCommandEvent& WXUNUSED(event) )
{
  frame_duration_dialog dlg(this);

  if ( dlg.ShowModal() == wxID_OK )
    {
      animation anim( m_animation_edit->get_value() );

      switch( dlg.get_mode() )
        {
        case frame_duration_dialog::duration_replace:
          for (std::size_t i=0; i!=anim.frames_count(); ++i)
            anim.get_frame(i).set_duration(dlg.get_operand());
          break;
        case frame_duration_dialog::duration_add:
          for (std::size_t i=0; i!=anim.frames_count(); ++i)
            anim.get_frame(i).set_duration
              ( dlg.get_operand() + anim.get_frame(i).get_duration() );
          break;
        case frame_duration_dialog::duration_multiply:
          for (std::size_t i=0; i!=anim.frames_count(); ++i)
            anim.get_frame(i).set_duration
              ( dlg.get_operand() * anim.get_frame(i).get_duration() );
          break;
        }

      m_animation_edit->set_value(anim);
    }
} // main_frame::on_change_duration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to an activation of the "online doc" menu.
 * \param event This event occured.
 */
void bf::main_frame::on_online_doc( wxCommandEvent& WXUNUSED(event) )
{
  wxLaunchDefaultBrowser
    ( wxT("http://plee-the-bear.sourceforge.net/" )
      wxT("wiki/index.php/%C9diteur_d'animations") );
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
      m_animation_edit->validate();

      if ( is_changed() )
        {
          wxMessageDialog dlg
            ( this,
              _("The animation is not saved."
                 " Do you want to save it now?"),
              _("Animation is not saved."), wxYES_NO | wxCANCEL );

          int answer = dlg.ShowModal();

          if ( answer == wxID_CANCEL )
            quit = false;
          else if ( answer == wxID_YES )
            quit = save();
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

  EVT_MENU( wxID_NEW, bf::main_frame::on_new_animation )
  EVT_MENU( wxID_OPEN, bf::main_frame::on_open_animation )
  EVT_MENU( wxID_SAVE, bf::main_frame::on_save )
  EVT_MENU( wxID_SAVEAS, bf::main_frame::on_save_as )

  EVT_MENU( bf::main_frame::ID_COMPILE, bf::main_frame::on_compile )

  EVT_MENU( wxID_EXIT, bf::main_frame::on_exit )

  EVT_MENU( bf::main_frame::ID_CHANGE_DURATION,
            bf::main_frame::on_change_duration )

  EVT_MENU( wxID_HELP, bf::main_frame::on_online_doc )
  EVT_MENU( wxID_ABOUT, bf::main_frame::on_about )

  EVT_CLOSE( bf::main_frame::on_close )
END_EVENT_TABLE()
