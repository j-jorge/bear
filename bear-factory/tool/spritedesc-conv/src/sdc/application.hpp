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

#include "xcf_map.hpp"

#include "spritedesc.hpp"

#include <claw/application.hpp>

namespace sdc
{
  /**
   * \brief The main class.
   *
   * The program takes a single parameter, which is the .spritedesc file. It
   * reads the description of the xcf files used in the .spritedesc file from
   * stdin and expects the following format:
   *
   * xcf_file_name
   * image_width image_height layer_count
   * layer_width layer_height layer_x layer_y layer_name
   *
   * Where the last line is repeated layer_count times. The description of the
   * xcf files must be repeated for each xcf file used in the .spritedesc
   * file.
   *
   * The program produces to results: it writes a Scheme script on stdout that
   * describes how to produce a png file from the xcf files according to the
   * .spritedesc file and it writes a .spritepos file describing the sprites
   * in the png file.
   * 
   * The resulting script uses functions from "common.scm" but does not load
   * it. Ensure that you load with a line like
   *
   *   (load "common.scm")
   *
   * when executing the result.
   *
   * \author Julien Jorge
   */
  class application:
    public claw::application
  {
  private:
    /**
     * \brief This function object is used to compare the sprites by decreasing
     *        order of their areas.
     */
    class sprite_area_comp
    {
    public:
      bool operator()
      ( const spritedesc::sprite& a, const spritedesc::sprite& b ) const;

    }; // class sprite_area_comp

    /**
     * \brief This function object is used to compare the sprites by decreasing
     *        order of their heights.
     */
    class sprite_height_comp
    {
    public:
      bool operator()
      ( const spritedesc::sprite& a, const spritedesc::sprite& b ) const;

    }; // class sprite_height_comp

    typedef claw::math::rectangle<std::size_t> rectangle_type;
    typedef std::list<std::string> path_list_type;

  public:
    application( int& argc, char** &argv );

    int run();

  private:
    void help() const;
    void check_arguments( int& argc, char** &argv );

    void process_file( const std::string& name );
    void read_layer_description( std::istream& is );

    void execute_gimp_scheme_process( std::string script ) const;

    void generate_output( const spritedesc& desc ) const;
    void generate_spritepos( std::ostream& os, const spritedesc& desc ) const;

    std::string get_scheme_path( std::string filename ) const;
    
    void generate_scm
    ( std::ostream& os, const spritedesc& desc ) const;

    void generate_scm
    ( std::ostream& os, const spritedesc::sprite& s,
      const std::string& target_id ) const;

    std::string make_image_varname( const std::string& id ) const;

    void set_sprite_position( spritedesc& desc ) const;
    bool find_place_for
    ( std::list<rectangle_type>& empty_places, spritedesc::sprite& s,
      std::size_t m ) const;

  private:
    /** \brief Tells if we should quit immediatly. */
    bool m_quit;

    /** \brief The sprite description file. */
    std::string m_input_file;

    /** \brief Tells to generate the spritepos file. */
    bool m_generate_spritepos;

    /** \brief The directories where the scheme scripts are searched. */
    path_list_type m_scheme_directory;

    /** \brief The path to gimp-console executable. */
    std::string m_gimp_console;

    /** \brief The description of the xcf files. */
    xcf_map m_xcf;

  }; // class application
} // namespace sdc

#endif // __SDC_APPLICATION_HPP__
