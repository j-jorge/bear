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
#include "parser.hpp"
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
#ifdef BEAR_SDC_DEFAULT_SCHEME_PATH
      m_scheme_directory.push_back( BEAR_SDC_DEFAULT_SCHEME_PATH );
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
      "If this argement is set, the images are not generated.", true );
  m_arguments.add
    ( "-x", "--xcfinfo", "The path to the xcfinfo executable.", true );
  m_arguments.add_long
    ( "--no-spritepos", "Tells to not generate the spritepos file.", true );

  m_arguments.parse( argc, argv );

  if ( m_arguments.get_bool("--help") || (argc == 0) )
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
      //makefile_generator g( m_makefile );
      //g.run( content );
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
  std::list<spritedesc> desc;

  if ( !p.run( xcf, desc, name ) )
    std::cerr << "Failed to process file '" << name << "'" << std::endl;

  return desc;
} // application::process_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Generates a makefile that calls the program to generate the images.
 * \param desc The descriptions of the images.
 */
void sdc::application::generate_makefile( std::list<spritedesc> desc ) const
{
  std::ostream* output;
  bool output_to_file(false);

  if ( m_makefile == "-" )
    output = &std::cout;
  else
    {
      output = new std::ofstream( m_makefile.c_str() );
      output_to_file = true;
    }

  std::vector<std::string> dependencies;

  if ( output_to_file )
    dependencies.push_back( m_makefile );

  // dependencies.insert( dependencies.end(), get_all_output_files( desc ) );

  *output << "all: " << boost::algorithm::join( dependencies, " " )
          << '\n';
  generate_makefile( *output, desc );

  if ( output_to_file )
    delete output;
} // application::generate_makefile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Generates a makefile that calls the program to generate the images.
 * \param output The stream in which the rules are writen.
 * \param desc The descriptions of the images.
 */
void sdc::application::generate_makefile
( std::ostream& output, std::list<spritedesc> desc ) const
{
  std::vector<std::string> output_files;
  std::vector<std::string> xcf_files;

  for ( std::list<spritedesc>::const_iterator it=desc.begin(); it!=desc.end();
        ++it )
    {
      output_files.push_back( it->output_name );

      for ( spritedesc::id_to_file_map::const_iterator xcf_it = it->xcf.begin();
            xcf_it != it->xcf.end(); ++xcf_it )
        xcf_files.push_back( xcf_it->second );
    }

  if ( output_files.empty() )
    return;

  output << make_image_name( output_files[0] );

  for ( std::size_t i=1; i!=output_files.size(); ++i )
    output << ' ' << make_image_name( output_files[i] );

  output << ": ";

  if ( !xcf_files.empty() )
    output << boost::algorithm::join( xcf_files, " " );

  output /* << ' ' << m_input_file */ << "\n";
  output << "\t" << m_arguments.get_program_name() << ' '
         << "--gimp-console=" << m_gimp_console_program << ' '
         << "--xcfinfo=" << m_xcfinfo_program;

  if ( !m_generate_spritepos )
    output << " --no-spritepos";

  for ( path_list_type::const_iterator it = m_scheme_directory.begin();
        it != m_scheme_directory.end(); ++it )
    output << " --scheme-directory=" << *it;

  output /* << ' ' << m_input_file */ << std::endl;
} // application::generate_makefile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the name of the image file generated for the spritesheet with a
 *        given name.
 * \param name The name of the spritesheet.
 */
std::string sdc::application::make_image_name( const std::string& name ) const
{
  return name + ".png";
} // application::make_image_name()
