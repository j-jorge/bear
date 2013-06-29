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
 * \brief Method called when the application is initializing.
 */
bool bf::base_editor_application::OnInit()
{
  bool result = false;

  if ( !show_help() )
    if ( !show_version() )
      {
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
          result = init_app();
      }

  return result;
} // base_editor_application::OnInit()

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
bool bf::base_editor_application::do_init_app()
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
 * \brief Initialise the application. minimal_init() is not called
 *        automatically.
 */
bool bf::base_editor_application::init_app()
{
  wxToolTip::Enable(true);

  return do_init_app();
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
        "\t--compile, -c\tCompile the files and exit, \n"
        "\t--update, -u\tUpdate the files and exit, \n"
        "\t--help, -h\tDisplay this help and exit, \n"
        "\t--version, -v\tDisplay the version of the program and exit."
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
  int index(0);
  bool stop(false);

  for (int i=1; !stop && (index==0) && (i<argc); ++i)
    if ( (argv[i] == long_name) || (argv[i] == short_name))
      index = i;
    else
      stop = wxString(argv[i]) == wxT("--");

  if ( index != 0 )
    {
      for ( int i=index; (i+1 != argc); ++i )
        argv[i] = argv[i+1];

      --argc;
      argv[argc] = NULL;
    }

  return index != 0;
} // base_editor_application::find_and_erase_option()
