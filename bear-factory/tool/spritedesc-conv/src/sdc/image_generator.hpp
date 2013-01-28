/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The image generator generates the images described by some
 *        spritedesc files.
 * \author Julien Jorge
 */
#ifndef __SDC_IMAGE_GENERATOR_HPP__
#define __SDC_IMAGE_GENERATOR_HPP__

#include "spritedesc.hpp"

namespace sdc
{
  /**
   * \brief The image generator generates the images described by some
   *        spritedesc files.
   * \author Julien Jorge
   */
  class image_generator
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

    /** \brief The structure that describes the rectangles when placing the
        sprites. */
    typedef claw::math::rectangle<std::size_t> rectangle_type;

    /**
     * \brief The paths to the directories where the files are searched and
     *        generated.
     */
    struct working_directory
    {
      /** \brief The directory where the xcf files are searched. */
      std::string xcf_directory;

      /** \brief The directory where the output is written. */
      std::string output_directory;
    }; // struct working_directory

  public:
    typedef std::list<spritedesc> spritedesc_collection;
    typedef std::map<std::string, spritedesc_collection> file_to_spritedesc_map;

    typedef std::list<std::string> path_list_type;

  public:
    image_generator
    ( bool generate_spritepos, path_list_type scheme_directory,
      std::string gimp_console_program );

    void run( file_to_spritedesc_map files );

  private:
    void process_spritedesc
    ( std::string source, spritedesc_collection desc ) const;

    void execute_gimp_scheme_process( std::string script ) const;

    void generate_output( working_directory dir, spritedesc desc ) const;
    void generate_spritepos( std::ostream& os, const spritedesc& desc ) const;

    std::string get_scheme_path( std::string filename ) const;
    
    void generate_scm
    ( std::ostream& os, working_directory dir, spritedesc desc ) const;

    void generate_scm
    ( std::ostream& os, const spritedesc::sprite& s,
      const std::string& target_id ) const;

    std::string make_image_varname( const std::string& id ) const;

    void set_sprite_position( spritedesc& desc ) const;
    bool find_place_for
    ( std::list<rectangle_type>& empty_places, spritedesc::sprite& s,
      std::size_t m ) const;

  private:
    /** \brief Tells to generate the spritepos file. */
    bool m_generate_spritepos;

    /** \brief The directories where the scheme scripts are searched. */
    path_list_type m_scheme_directory;

    /** \brief The path to gimp-console executable. */
    std::string m_gimp_console_program;

  }; // class image_generator
} // namespace sdc

#endif // __SDC_IMAGE_GENERATOR_HPP__
