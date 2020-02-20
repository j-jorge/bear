/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::base_editor_application class.
 * \author Julien Jorge.
 */
#include "bf/base_editor_application.hpp"

#include "bf/path_configuration.hpp"
#include "bf/version.hpp"
#include "bf/workspace_environment.hpp"
#include "bf/wx_facilities.hpp"

#include <wx/tooltip.h>
#include <iostream>

#include <claw/exception.hpp>
#include <claw/logger.hpp>
#include <boost/preprocessor/stringize.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::base_editor_application::base_editor_application()
  : m_locale( wxLocale::GetSystemLanguage() )
{
  claw::logger.set( new claw::console_logger() );
  claw::logger.set_level( claw::log_verbose );

#ifdef BEAR_FACTORY_TEXT_DOMAIN_PATH
  m_locale.AddCatalogLookupPathPrefix
    ( L"" BOOST_PP_STRINGIZE(BEAR_FACTORY_TEXT_DOMAIN_PATH) );
#endif

  m_locale.AddCatalog( wxT("bear-factory") );
} // base_editor_application::base_editor_application()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::base_editor_application::~base_editor_application()
{
  // frames are deleted by wxWidgets

  claw::logger.clear();
} // base_editor_application::~base_editor_application()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile a file.
 * \param path The path to the file.
 */
void bf::base_editor_application::compile( const wxString& path ) const
{

} // base_editor_application::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update a file.
 * \param path The path to the file.
 */
void bf::base_editor_application::update( const wxString& path ) const
{

} // base_editor_application::update()

/*----------------------------------------------------------------------------*/
/**
 * \brief Application-defined initialisation.
 */
bool bf::base_editor_application::do_init_app(const workspace_environment& env)
{
  return true;
} // base_editor_application::do_init_app()

/*----------------------------------------------------------------------------*/
/**
 * \brief Application-defined minimal initialisation, for command line usage.
 */
bool bf::base_editor_application::do_command_line_init()
{
  return true;
} // base_editor_application::do_command_line_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when the application is initializing.
 */
bool bf::base_editor_application::OnInit()
{
  bool result = false;

  if ( show_help() )
    return result;

  if ( show_version() )
    return result;

  const bool compile_f
    ( find_and_erase_option( wxT("--compile"), wxT("-c") ) );
  const bool update_f
    ( find_and_erase_option( wxT("--update"), wxT("-u") ) );

  if ( compile_f || update_f )
    {
      command_line_init();

      if ( update_f )
        update_arguments();

      if ( compile_f )
        compile_arguments();
    }
  else
    {
      std::string workspace_name;

      get_worspace_name( workspace_name );

      result = init_app( workspace_environment( workspace_name ) );
    }
      
  return result;
} // base_editor_application::OnInit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the name of the worspace if the user has given one.
 * \param result The name of the workspace, as provided by the user.
 * \return true if a workspace has been given.
 */
bool bf::base_editor_application::get_worspace_name( std::string& result )
{
  std::string workspace_name;
  bool result_is_set( false );

  const bool has_workspace
    ( find_and_erase_option_value
      ( wxT("--workspace"), wxT("-w"), workspace_name ) );

  if ( has_workspace )
    {
      result = workspace_name;
      result_is_set = true;
    }

  return result_is_set;
} // base_editor_application::get_worspace_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the application. minimal_init() is not called
 *        automatically.
 * \param env The workspace environment. 
 */
bool bf::base_editor_application::init_app(const workspace_environment& env)
{
  wxToolTip::Enable(true);

  return do_init_app(env);
} // base_editor_application::init_app()

/*----------------------------------------------------------------------------*/
/**
 * \brief Minimal initialisation of the application, for command line usage.
 */
bool bf::base_editor_application::command_line_init()
{
  return do_command_line_init();
} // base_editor_application::command_line_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the files and exit.
 */
bool bf::base_editor_application::compile_arguments() const
{
  bool result(true);

  for (int i=1; i<argc; ++i)
    if ( wxString(argv[i]) != wxT("--") )
      try
        {
          claw::logger << claw::log_verbose << "Compiling "
                       << wx_to_std_string(argv[i]) << std::endl;
          compile(argv[i]);
        }
      catch(std::exception& e)
        {
          std::cerr << "Error when processing '" << argv[i] << "': "
                    << e.what() << std::endl;
          result = false;
        }

  return result;
} // base_editor_application::compile_arguments()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the files and exit.
 */
bool bf::base_editor_application::update_arguments() const
{
  bool result(true);

  for (int i=1; i<argc; ++i)
    if ( wxString(argv[i]) != wxT("--") )
      try
        {
          claw::logger << claw::log_verbose << "Updating "
                       << wx_to_std_string(argv[i]) << std::endl;
          update(argv[i]);
        }
      catch(std::exception& e)
        {
          std::cerr << "Error when processing '" << argv[i] << "': "
                    << e.what() << std::endl;
          result = false;
        }

  return result;
} // base_editor_application::update_arguments()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show the command line usage.
 * \return true if the usage has been shown.
 */
bool bf::base_editor_application::show_help()
{
  if ( find_and_erase_option(wxT("--help"), wxT("-h")) )
    {
      std::cout << "usage:\n" << wx_to_std_string(argv[0])
                << " [option] [file...]\n"
        "Where the options are:\n\n"
        "\t--compile, -c\n\t\tCompile the files and exit, \n"
        "\t--update, -u\n\t\tUpdate the files and exit, \n"
        "\t--workspace, -w string\n\t\tWhen no file is provided, "
        "create a new editor in this workspace, \n"
        "\t--help, -h\n\t\tDisplay this help and exit, \n"
        "\t--version, -v\n\t\tDisplay the version of the program and exit."
                << std::endl;
      return true;
    }
  else
    return false;
} // base_editor_application::show_help()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show the version of the program
 * \return true if the version has been shown.
 */
bool bf::base_editor_application::show_version()
{
  if ( find_and_erase_option(wxT("--version"), wxT("-v")) )
    {
      std::cout << BF_VERSION_STRING << std::endl;
      return true;
    }
  else
    return false;
} // base_editor_application::show_version()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if an option is present on the command line and remove it.
 * \param long_name The long name of the option.
 * \param short_name The short name of the option.
 */
bool bf::base_editor_application::find_and_erase_option
( const wxString& long_name, const wxString& short_name )
{
  const int index( find_option_index(long_name, short_name) );

  if ( index != argc )
    {
      remove_options_at( index, 1 );
      return true;
    }

  return false;
} // base_editor_application::find_and_erase_option()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if an option is present on the command line and has an
 *        associated value, then remove it.
 * \param long_name The long name of the option.
 * \param short_name The short name of the option.
 * \param result The value associated with the option.
 * \return true if the option and its value has been found.
 */
bool bf::base_editor_application::find_and_erase_option_value
( const wxString& long_name, const wxString& short_name, std::string& result )
{
  const int index( find_option_index(long_name, short_name) );

  if ( (index + 1 < argc) )
    {
      result = wx_to_std_string( argv[index + 1] );

      remove_options_at( index, 2 );

      return true;
    }

  return false;
} // base_editor_application::find_and_erase_option_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Finds the index of a given program option in the program options.
 * \param long_name The long name of the option.
 * \param short_name The short name of the option.
 * \return The index of the option if it has been found, argc otherwise.
 */
int bf::base_editor_application::find_option_index
( const wxString& long_name, const wxString& short_name ) const
{
  bool stop(false);

  for (int i=1; !stop && (i<argc); ++i)
    if ( (argv[i] == long_name) || (argv[i] == short_name))
      return i;
    else
      stop = wxString(argv[i]) == wxT("--");

  return argc;
} // base_editor_application::find_option_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Removes some program options starting from a given position.
 * \param index The index of the first option to remove.
 * \param count The number of options to remove.
 */
void bf::base_editor_application::remove_options_at( int index, int count )
{
  for ( int i=index; (i+count <= argc); ++i )
    argv[i] = argv[i + count];

  for ( int i=0; i != count; ++i )
    {
      --argc;
      argv[argc] = (char*)NULL;
    }
} // base_editor_application::remove_options_at()
