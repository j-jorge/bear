/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the sdc::application class.
 * \author Julien Jorge
 */
#include "application.hpp"

#include <fstream>
#include <iostream>

#include "image_generator.hpp"
#include "makefile_generator.hpp"
#include "parser.hpp"
#include "version.hpp"
#include "xcf_map.hpp"

#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string/join.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param argc Number of program arguments.
 * \param argv Program arguments.
 */
sdc::application::application( int& argc, char** &argv )
  : claw::application(argc, argv), m_quit(false), m_generate_spritepos(true),
    m_gimp_console_program( "gimp-console" ), m_xcfinfo_program( "xcfinfo" )
{
  check_arguments( argc, argv );
} // application::application()

/*----------------------------------------------------------------------------*/
/**
 * \brief Run the application : split the image.
 */
int sdc::application::run()
{
  if (!m_quit)
    {
#ifdef SDC_DEFAULT_SCHEME_PATH
      m_scheme_directory.push_back( SDC_TO_STR( SDC_DEFAULT_SCHEME_PATH ) );
#endif

      process_files();
    }

  return 0;
} // application::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Print program usage.
 */
void sdc::application::help() const
{
  m_arguments.help("file.spritedesc...");
} // application::help()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check program arguments.
 * \param argc Number of program arguments.
 * \param argv Program arguments.
 */
void sdc::application::check_arguments( int& argc, char** &argv )
{
  m_arguments.add( "-h", "--help", "Print this message and exit.", true );
  m_arguments.add
    ( "-s", "--scheme-directory",
      "A directory where the utility Scheme scripts can be found.", true );
  m_arguments.add
    ( "-g", "--gimp-console",
      "The path to the gimp-console executable.", true );
  m_arguments.add
    ( "-m", "--makefile",
      "The name of the makefile to generate. "
      "If this argument is set, the images are not generated.", true );
  m_arguments.add
    ( "-t", "--target",
      "The name of the sprite sheet to generate from the input file.", true );
  m_arguments.add
    ( "-x", "--xcfinfo", "The path to the xcfinfo executable.", true );
  m_arguments.add_long
    ( "--no-spritepos", "Tells to not generate the spritepos file.", true );
  m_arguments.add_long
    ( "--version", "Prints the version of the software.", true );

  m_arguments.parse( argc, argv );

  if ( m_arguments.get_bool("--version") )
    {
      std::cout << SDC_VERSION_STRING << std::endl;
      m_quit = true;
    }

  if ( !m_quit && (m_arguments.get_bool("--help") || (argc == 0)) )
    {
      help();
      m_quit = true;
    }

  if ( m_arguments.has_value("--gimp-console") )
    m_gimp_console_program = m_arguments.get_string("--gimp-console");

  if ( m_arguments.has_value("--makefile") )
    m_makefile = m_arguments.get_string("--makefile");

  if ( m_arguments.has_value("--scheme-directory") )
    m_scheme_directory = m_arguments.get_all_of_string("--scheme-directory");

  m_generate_spritepos = !m_arguments.get_bool("--no-spritepos");

  for ( int argi=0; argi!=argc; ++argi )
    m_input_file.push_back( argv[argi] );

  if ( m_arguments.has_value( "--target" ) )
    m_target = m_arguments.get_all_of_string( "--target" );
} // application::check_arguments()

/*----------------------------------------------------------------------------*/
/**
 * \brief Processes the spritedesc files.
 */
void sdc::application::process_files()
{
  file_to_spritedesc_map content;

  for ( std::size_t i=0; i!=m_input_file.size(); ++i )
    content[ m_input_file[i] ] = process_file( m_input_file[i] );

  if ( m_makefile.empty() )
    {
      image_generator g
        ( m_generate_spritepos, m_scheme_directory, m_gimp_console_program );
      g.run( content );
    }
  else
    {
      makefile_generator g( m_makefile, get_self_command() );
      g.run( content );
    }
} // application::process_files()

/*----------------------------------------------------------------------------*/
/**
 * \brief Process a file.
 * \param name The name of the file to process.
 */
sdc::application::spritedesc_collection
sdc::application::process_file( std::string name ) const
{
  const boost::filesystem::path file_path( name, boost::filesystem::native );
  const boost::filesystem::path file_directory( file_path.parent_path() );
  xcf_map xcf( file_directory.string(), m_xcfinfo_program );

  parser p;
  spritedesc_collection desc;

  if ( !p.run( xcf, desc, name ) )
    std::cerr << "Failed to process file '" << name << "'" << std::endl;

  spritedesc_collection result;

  if ( m_target.empty() )
    result = desc;
  else
    for ( spritedesc_collection::const_iterator it = desc.begin();
          it != desc.end(); ++it )
      if ( std::find( m_target.begin(), m_target.end(), it->output_name )
           != m_target.end() )
        result.push_back( *it );

  return result;
} // application::process_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the command to pass to the makefile generator to execute this
 *        program.
 */
std::string sdc::application::get_self_command() const
{
  std::ostringstream result;

  result << m_arguments.get_program_name() << ' '
         << "--gimp-console=" << m_gimp_console_program << ' '
         << "--xcfinfo=" << m_xcfinfo_program;

  if ( !m_generate_spritepos )
    result << " --no-spritepos";

  for ( path_list_type::const_iterator it = m_scheme_directory.begin();
        it != m_scheme_directory.end(); ++it )
    result << " --scheme-directory=" << *it;

  return result.str();
} // application::get_self_command()
