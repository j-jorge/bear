/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The main class.
 * \author Julien Jorge
 */
#ifndef __SDC_APPLICATION_HPP__
#define __SDC_APPLICATION_HPP__

#include "spritedesc.hpp"

#include <claw/application.hpp>

#include <vector>

namespace sdc
{
  /**
   * \brief The main class.
   * \author Julien Jorge
   */
  class application:
    public claw::application
  {
  private:
    typedef std::list<spritedesc> spritedesc_collection;
    typedef std::map<std::string, spritedesc_collection> file_to_spritedesc_map;

    typedef std::list<std::string> path_list_type;

  public:
    application( int& argc, char** &argv );

    int run();

  private:
    void help() const;
    void check_arguments( int& argc, char** &argv );

    void process_files();
    spritedesc_collection process_file( std::string name ) const;

    std::string get_self_command() const;

  private:
    /** \brief Tells if we should quit immediatly. */
    bool m_quit;

    /** \brief The sprite description file. */
    std::vector<std::string> m_input_file;

    /** \brief The images to generate from the input files. */
    std::list<std::string> m_target;

    /** \brief Tells to generate the spritepos file. */
    bool m_generate_spritepos;

    /** \brief The directories where the scheme scripts are searched. */
    path_list_type m_scheme_directory;

    /** \brief The path to gimp-console executable. */
    std::string m_gimp_console_program;

    /** \brief The path to xcfinfo executable. */
    std::string m_xcfinfo_program;

    /** \brief The name of the makefile to generate, if any. */
    std::string m_makefile;

  }; // class application
} // namespace sdc

#endif // __SDC_APPLICATION_HPP__
