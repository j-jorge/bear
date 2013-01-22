/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::running_bear class.
 * \author Julien Jorge
 */
#include "running_bear.hpp"

#include "bear_gettext.hpp"

#include <claw/logger.hpp>
#include <boost/preprocessor/stringize.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param argc Number of program arguments.
 * \param argv Program arguments.
 */
bear::running_bear::running_bear( int& argc, char** &argv )
  : claw::application(argc, argv), m_game(NULL)
{
#ifdef BEAR_TEXT_DOMAIN_PATH
  bindtextdomain( "bear-engine", BOOST_PP_STRINGIZE(BEAR_TEXT_DOMAIN_PATH) );
#endif

  bind_textdomain_codeset( "bear-engine", "ISO-8859-15" );
  textdomain( "bear-engine" );

  m_arguments.add
    ("-h", "--help", bear_gettext("Print this help screen and exit."), true);
  m_arguments.parse(argc, argv);

  if ( m_arguments.get_bool("--help") )
    help();
  else
    create_game( argc, argv );
} // running_bear::running_bear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::running_bear::~running_bear()
{
  delete m_game;
} // running_bear::~running_bear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Run the application.
 */
int bear::running_bear::run()
{
  try
    {
      if ( m_game != NULL )
        {
          m_game->run();
          delete m_game;
          m_game = NULL;
        }

      return EXIT_SUCCESS;
    }
  catch( std::exception& e )
    {
      claw::logger << claw::log_error << "(exception) " << e.what()
                   << std::endl;
      delete m_game;
      m_game = NULL;
      return EXIT_FAILURE;
    }
} // running_bear::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the game.
 * \param argc Number of program arguments.
 * \param argv Program arguments.
 */
void bear::running_bear::create_game( int& argc, char** &argv )
{
  try
    {
      m_game = new engine::game( argc, argv );
    }
  catch( std::exception& e )
    {
      std::cerr << "Exception: " << e.what() << std::endl;
      help();
    }
} // running_bear::create_game()

/*----------------------------------------------------------------------------*/
/**
 * \brief Print some help about the usage of the program.
 */
void bear::running_bear::help() const
{
  m_arguments.help( bear_gettext("engine_options") );
  std::cout << '\n';
  engine::game::print_help();
} // running_bear::help()
