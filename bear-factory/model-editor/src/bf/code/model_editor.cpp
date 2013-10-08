/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::model_editor class.
 * \author Julien Jorge.
 */
#include "bf/model_editor.hpp"

#include "bf/model_file_compiler.hpp"
#include "bf/config_frame.hpp"
#include "bf/gui_model.hpp"
#include "bf/main_frame.hpp"
#include "bf/path_configuration.hpp"
#include "bf/version.hpp"
#include "bf/workspace_environment.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/xml/model_file.hpp"

#include <wx/tooltip.h>
#include <claw/logger.hpp>
#include <claw/exception.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::model_editor::model_editor()
  : m_main_frame(NULL)
{
} // model_editor::model_editor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the configuration of the program.
 */
bf::configuration& bf::model_editor::get_config()
{
  return m_config;
} // model_editor::get_config()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the configuration of the program.
 */
const bf::configuration& bf::model_editor::get_config() const
{
  return m_config;
} // model_editor::get_config()

/*----------------------------------------------------------------------------*/
/**
 * \brief Configure the program.
 */
void bf::model_editor::configure()
{
  config_frame dlg(NULL);

  if ( dlg.ShowModal() == wxID_OK )
    {
      // TO DO
      // Call update_image_pool() for each main_frame 
      ; 
    }
} // model_editor::configure()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile a model.
 * \param path The path to the model file.
 */
void bf::model_editor::compile( const wxString& path ) const
{
  model* mdl(NULL);

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
          
          if ( check_model(*mdl) )
            {
              compile_model(*mdl, path);
              delete mdl;
            }
          else
            {
              delete mdl;
              claw::exception("Invalid model.");
            }
        }
    }
  catch(...)
    {
      delete mdl;
      throw;
    }
} // model_editor::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update a model.
 * \param path The path to the model file.
 */
void bf::model_editor::update( const wxString& path ) const
{
  gui_model* mdl(NULL);

  try
    {
      std::string w = 
        path_configuration::get_instance().search_workspace
        ( wx_to_std_string(path) );
      
      if ( ! w.empty() )
        {
          workspace_environment env(w);
          
          xml::model_file mf;
          mdl = mf.load(path, &env);
          
          std::ofstream f( wx_to_std_string(path).c_str() );
          mf.save(*mdl, f);
        }
         
      delete mdl;
    }
  catch(...)
    {
      delete mdl;
      throw;
    }
} // model_editor::update()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile a model.
 * \param mdl The model to compile.
 * \param path The path to the model file.
 * \return true if the compilation went ok.
 */
bool
bf::model_editor::compile_model
( const model& mdl, const wxString& path ) const
{
  bool result(true);

  try
    {
      model_file_compiler c;

      std::string w = 
        path_configuration::get_instance().search_workspace
        ( wx_to_std_string(path) );
      
      if ( ! w.empty() )
        {
          workspace_environment env(w);
          c.compile( mdl, wx_to_std_string(path), &env );
        }
    }
  catch(...)
    {
      result = false;
    }

  return result;
} // model_editor::compile_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the application.
 */
bool bf::model_editor::do_init_app()
{
  init_config();

  m_main_frame = new main_frame;
  m_main_frame->Show();

  load_models();

  return true;
} // model_editor::do_init_app()

/*----------------------------------------------------------------------------*/
/**
 * \brief Minimal initialisation for command line usage.
 */
bool bf::model_editor::do_command_line_init()
{
  init_config();

  return true;
} // model_editor::do_command_line_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load and apply the configuration.
 */
void bf::model_editor::init_config()
{
  m_config.load();
} // model_editor::init_config()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if a model is valid.
 * \param mdl The model to check.
 */
bool bf::model_editor::check_model( const model& mdl ) const
{
  bool result(true);
  model::const_action_iterator it;

  for ( it=mdl.action_begin(); it!=mdl.action_end(); ++it )
    if ( !it->get_auto_next().empty() )
      if ( !mdl.has_action( it->get_auto_next() )  )
        {
          result = false;
          claw::logger << claw::log_error << "Unknown action '"
                       << it->get_auto_next() << '\'' << std::endl;
        }

  return result;
} // model_editor::check_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the models passed on the command line.
 */
void bf::model_editor::load_models()
{
  for (int i=1; i<argc; ++i)
    if ( wxString(argv[i]) != wxT("--") )
      try
        {
          m_main_frame->load_model( argv[i] );
        }
      catch( std::ios_base::failure& e )
        {
          claw::logger << claw::log_error << e.what()
                       << "\nCreating a new model." << std::endl;
          m_main_frame->new_model( argv[i] );
        }
      catch( std::exception& e )
        {
          claw::logger << claw::log_error << e.what() << std::endl;
        }
} // level_editor::load_levels()
