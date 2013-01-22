/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::level_runner class.
 * \author Julien Jorge
 */
#include "bf/level_runner.hpp"

#include "bf/compilation_context.hpp"
#include "bf/level_check_result.hpp"
#include "bf/path_configuration.hpp"
#include "bf/run_configuration.hpp"

#include "bf/wx_facilities.hpp"

#include <fstream>

#include <wx/filename.h>
#include <wx/utils.h>
#include <claw/exception.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param config The configuration to use to run the game.
 * \param lvl The level to run.
 * \param layer_index The index of the layer in which the items are inserted.
 * \param x The x-origin of the items to insert.
 * \param y The y-origin of the items to insert.
 */
bf::level_runner::level_runner
( const run_configuration& config, const level& lvl, std::size_t layer_index,
  double x, double y )
  : m_config(config), m_level(lvl), m_layer_index(layer_index), m_x(x), m_y(y)
{

} // level_runner::level_runner()

/*----------------------------------------------------------------------------*/
/**
 * \brief Run the level.
 */
void bf::level_runner::run()
{
  initialize_run();

  wxFileName::Mkdir(get_data_directory(), 0777, wxPATH_MKDIR_FULL);
  const wxString level_file =
    wxFileName::CreateTempFileName(get_data_directory());
  compile_level( level_file );

  const wxString cwd = wxFileName::GetCwd();
  set_working_directory();

  wxChar** argv = create_argv( wxFileName(level_file).GetFullName() );

  const long result = wxExecute( argv );
  delete_argv(argv);
  wxFileName::SetCwd(cwd);

  if ( result < 0 )
    throw claw::exception
      ( wx_to_std_string( _("Could not run the level.") ) );
} // level_runner::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the environment before the run.
 */
void bf::level_runner::initialize_run()
{
  if ( !add_temporary_items() )
    throw claw::exception
      ( wx_to_std_string( _("Could not add the items in the layer.") ) );

  level_check_result check_result;
  m_level.check(check_result);

  if ( !check_result.is_ok() )
    throw claw::exception
      ( wx_to_std_string
        ( _("There were some errors when checking the level.") ) );
} // level_runner::initialize_run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the level to run.
 * \param p The path of the file in which the level is compiled.
 */
void bf::level_runner::compile_level( const wxString& p ) const
{
  std::ofstream f( wx_to_std_string(p).c_str() );
  compiled_file output(f);
  compilation_context context(0);
  m_level.compile(output, context);
} // level_runner::compile_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the temporary items to the layer.
 */
bool bf::level_runner::add_temporary_items() const
{
  if ( m_layer_index >= m_level.layers_count() )
    return false;

  for ( std::size_t i=0; i!=m_config.get_items_count(); ++i )
    {
      item_instance* item = new item_instance(m_config.get_item(i));
      item->get_rendering_parameters().set_position
        ( m_x + item->get_rendering_parameters().get_left(),
          m_y + item->get_rendering_parameters().get_bottom() );

      m_level.get_layer(m_layer_index).add_item(item);
    }

  return true;
} // level_runner::add_temporary_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the directory in which the temporary level is saved.
 */
wxString bf::level_runner::get_data_directory() const
{
  return wxFileName::GetTempDir() + wxT("/bear-factory/");
} // level_runner::get_data_directory()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the arguments of the program to run.
 * \param level_file The level file to load at the beginning of the game.
 */
wxChar** bf::level_runner::create_argv( const wxString& level_file ) const
{
  std::vector<wxChar*> arguments;

  arguments.push_back
    ( alloc_argument(std_to_wx_string(m_config.get_program_path())) );

  const std::string var("{level_file}");
  for ( std::size_t i=0; i!=m_config.get_arguments_count(); ++i )
    {
      std::string arg = m_config.get_argument(i);
      std::size_t p = arg.find(var);

      if ( p != std::string::npos )
        arg.replace( p, var.length(), wx_to_std_string(level_file) );

      arguments.push_back( alloc_argument(std_to_wx_string(arg)) );
    }

  arguments.push_back
    ( alloc_argument( wxT("--data-path=") + get_data_directory()) );
  arguments.push_back
    ( alloc_argument( wxT("--start-level=") + level_file) );

  wxChar** result = new wxChar*[arguments.size() + 1];

  for ( std::size_t i=0; i!=arguments.size(); ++i )
    result[i] = arguments[i];

  result[arguments.size()] = NULL;
  return result;
} // level_runner::create_argv()

/*----------------------------------------------------------------------------*/
/**
 * \brief Delete the arguments passed to the program.
 */
void bf::level_runner::delete_argv( wxChar** argv ) const
{
  for ( std::size_t i=0; argv[i] != NULL; ++i )
    delete[] argv[i];

  delete[] argv;
} // level_runner::delete_argv()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate an argument to be passed to the process.
 * \param arg The string from which the argument is created.
 */
wxChar* bf::level_runner::alloc_argument( const wxString& arg ) const
{
  wxChar* result = new wxChar[ arg.size() + 1 ];
  wcscpy( result, arg.c_str() );
  return result;
} // level_runner::alloc_argument()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the working directory to run the level.
 */
void bf::level_runner::set_working_directory() const
{
  const wxFileName f( std_to_wx_string(m_config.get_program_path()) );
  wxFileName::SetCwd( f.GetPath() );
} // level_runner::alloc_argument()
