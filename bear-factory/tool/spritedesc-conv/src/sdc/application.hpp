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
    std::string m_gimp_console_program;

    /** \brief The path to xcfinfo executable. */
    std::string m_xcfinfo_program;

  }; // class application
} // namespace sdc

#endif // __SDC_APPLICATION_HPP__
