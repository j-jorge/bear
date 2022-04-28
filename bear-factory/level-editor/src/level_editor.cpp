/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::level_editor class.
 * \author Julien Jorge.
 */
#include "bf/level_editor.hpp"

#include "bf/compilation_context.hpp"
#include "bf/config_frame.hpp"
#include "bf/gui_level.hpp"
#include "bf/image_pool.hpp"
#include "bf/layer_check_result.hpp"
#include "bf/level_check_result.hpp"
#include "bf/level_file_xml_reader.hpp"
#include "bf/level_file_xml_writer.hpp"
#include "bf/main_frame.hpp"
#include "bf/path_configuration.hpp"
#include "bf/splash_screen.hpp"
#include "bf/level_editor.hpp"
#include "bf/workspace_environment.hpp"
#include "bf/wx_facilities.hpp"

#include <wx/tooltip.h>
#include <claw/logger/logger.hpp>
#include <claw/exception.hpp>

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::level_editor::level_editor()
  : m_main_frame(NULL)
{
  srand(time(NULL));
} // level_editor::level_editor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the configuration of the program.
 */
bf::configuration& bf::level_editor::get_config()
{
  return m_config;
} // level_editor::get_config()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the configuration of the program.
 */
const bf::configuration& bf::level_editor::get_config() const
{
  return m_config;
} // level_editor::get_config()

/*----------------------------------------------------------------------------*/
/**
 * \brief Configure the program.
 */
void bf::level_editor::configure()
{
  config_frame dlg(NULL);

  if ( dlg.ShowModal() == wxID_OK )
    {
      //TO DO
      // inform that the application must be reloaded
      ;
    }
} // level_editor::configure()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile a level.
 * \param path The path to the level file.
 */
void bf::level_editor::compile( const wxString& path ) const
{
  level_file_xml_reader reader;
  level* lvl(NULL);

  try
    {
      std::string w = 
        path_configuration::get_instance().search_workspace
        ( wx_to_std_string(path) );
      
      if ( ! w.empty() )
        {
          workspace_environment env(w);

          lvl = reader.load( path, env );

          if ( check_level(*lvl) )
            {
              compile_level(*lvl, path);
              delete lvl;
            }
          else
            {
              delete lvl;
              claw::exception("Invalid level.");
            }
        }
    }
  catch(...)
    {
      delete lvl;
      throw;
    }
} // level_editor::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update a level.
 * \param path The path to the level file.
 */
void bf::level_editor::update( const wxString& path ) const
{
  level_file_xml_reader reader;
  level_file_xml_writer writer;
  level* lvl(NULL);

  try
    {
      std::string w = 
        path_configuration::get_instance().search_workspace
        ( wx_to_std_string(path) );
      
      if ( ! w.empty() )
        {
          workspace_environment env(w);
          
          lvl = reader.load( path, env );

          std::ofstream f( wx_to_std_string(path).c_str() );
          writer.save( f, *lvl, env );
          delete lvl;
        }
    }
  catch(...)
    {
      delete lvl;
      throw;
    }
} // level_editor::update()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile a level.
 * \param lvl The level to compile.
 * \param path The path to the level file.
 * \return true if the compilation went ok.
 */
void bf::level_editor::compile_level
( const level& lvl, const wxString& path ) const
{
  std::string std_path( wx_to_std_string(path) );
  std::size_t pos = std_path.rfind(".lvl");

  if ( pos != std::string::npos )
    std_path = std_path.substr(0, pos);

  std_path += ".cl";

  std::ofstream f( std_path.c_str() );

  if (f)
    {
      std::string w = 
        path_configuration::get_instance().search_workspace
        ( wx_to_std_string(path) );
      
      if ( ! w.empty() )
        {
          workspace_environment env(w);
          
          compiled_file cf(f);
          compilation_context context
            ( std::numeric_limits<unsigned int>::max(), env );
          lvl.compile(cf, context);
        }
    }
  else
    throw claw::exception("Can't open the level file.");
} // level_editor::compile_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when the application is initializing.
 * \param default_env Default workspace_environment.
 */
bool bf::level_editor::do_init_app(const workspace_environment & default_env)
{
  splash_screen* splash = splash_screen::create(wxT("level-splash.png"));

  if (splash!=NULL)
    {
      splash->get_version_label().SetForegroundColour( *wxWHITE );
      splash->get_status_label().SetForegroundColour( *wxWHITE );
      splash->get_status_label().SetLabel(_("Loading item class files..."));
      wxYield();
    }

  init_config();

  if (splash!=NULL)
    {
      splash->get_status_label().SetLabel(_("Loading images..."));
      wxYield();
    }

  m_main_frame = new main_frame;
  m_main_frame->Show();

  if (splash!=NULL)
    {
      splash->get_status_label().SetLabel(_("Loading levels..."));
      wxYield();
    }

  load_levels();

  if (splash!=NULL)
    splash->Destroy();

  return true;
} // level_editor::do_init_app()

/*----------------------------------------------------------------------------*/
/**
 * \brief Minimal initialisation for command line usage.
 */
bool bf::level_editor::do_command_line_init()
{
  init_config();

  return true;
} // level_editor::do_command_line_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load and apply the configuration.
 */
void bf::level_editor::init_config()
{
  m_config.load();
} // level_editor::init_config()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if a level is valid.
 * \param lvl The level to check.
 */
bool bf::level_editor::check_level( level& lvl ) const
{
  level_check_result result;

  lvl.check(result);

  for ( level_check_result::layer_iterator it=result.layer_begin();
        it!=result.layer_end(); ++it )
    print_layer_check_result( it->first, it->second );

  return result.is_ok();
} // level_editor::check_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Display the check errors of a layer.
 * \param lay The checked layer.
 * \param result The result of the check.
 */
void bf::level_editor::print_layer_check_result
( const layer* lay, const layer_check_result& result ) const
{
  wxPuts
    ( wxString::Format
      ( _("In layer '%s':\n"),
        wxGetTranslation(std_to_wx_string(lay->get_class_name())) ) );

  for ( layer_check_result::item_iterator it=result.item_begin();
      it!=result.item_end(); ++it )
    print_item_check_result( it->first, it->second );
} // level_editor::print_layer_check_result()

/*----------------------------------------------------------------------------*/
/**
 * \brief Display the check errors of an item.
 * \param item The checked item.
 * \param result The result of the check.
 */
void bf::level_editor::print_item_check_result
( const item_instance* item, const item_check_result& result ) const
{
  wxPuts
  ( wxString::Format
      ( _("In item '%s' at (%f, %f):\n"),
          item->get_class().get_class_name().c_str(),
          item->get_rendering_parameters().get_left(),
          item->get_rendering_parameters().get_bottom() ) );

  for ( item_check_result::const_iterator it=result.begin();
      it!=result.end(); ++it )
    if ( it->get_cause().empty() )
      wxPuts( wxGetTranslation(std_to_wx_string( it->get_message() )) );
    else
      wxPuts
      ( wxString::Format
        ( _("%s: %s\n"),
          wxGetTranslation(std_to_wx_string(it->get_cause())),
          wxGetTranslation(std_to_wx_string(it->get_message())) ) );
} // level_editor::print_item_check_result()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the levels passed on the command line.
 */
void bf::level_editor::load_levels()
{
  for (int i=1; i<argc; ++i)
    try
      {
        m_main_frame->load_level( argv[i] );
      }
    catch( std::ios_base::failure& e )
      {
        claw::logger << claw::log_error << e.what()
                     << "\nCreating a new level." << std::endl;
        m_main_frame->new_level( argv[i] );
      }
    catch( std::exception& e )
      {
        claw::logger << claw::log_error << e.what() << std::endl;
      }
} // level_editor::load_levels()
