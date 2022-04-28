/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The main class.
 * \author Julien Jorge
 */
#ifndef __PTB_IC_APPLICATION_HPP__
#define __PTB_IC_APPLICATION_HPP__

#include <claw/application.hpp>
#include <claw/graphic/image.hpp>
#include <claw/math.hpp>

#include <string>
#include <set>

namespace ptb
{
  namespace ic
  {
    /**
     * \brief The main class.
     *
     * The program accept the following parameters :
     * - --output-as-layer Output the declaration for a decoration layer,
     * - --output-folder Path to the directory where we save the images,
     * - --output-format The type of the output files,
     * - -x X-coordinate of the picture in the level,
     * - -y Y-coordinate of the picture in the level,
     * - -max-x maximum size of the sub images on the X-coordinates,
     * - -max-y maximum size of the sub images on the Y-coordinates,
     * - --file-prefix the prefix of the name of the files,
     * - --help Print help and exit.
     * \author Julien Jorge
     */
    class application:
      public claw::application
    {
    private:
      /**
       * \brief informations on the sub images.
       */
      struct sub_image_info
      {
  /** \brief The position and the size of the sub image in the source
      image. */
  claw::math::rectangle<unsigned int> box;

  /** \brief The path of the file in which we save the sub image. */
  std::string path;

      }; // struct sub_image_info

    public:
      application( int& argc, char** &argv );

      int run();

    private:
      void help() const;
      void check_arguments( int& argc, char** &argv );
      void store_files( int& argc, char** &argv );

      void process_file( const std::string& name );
      void process_file( std::istream& is, const std::string& name );

      claw::graphic::image* load_image( std::istream& is ) const;
      void split_image( const claw::graphic::image& img,
      const std::string& name ) const;

      void save_image( const claw::graphic::image& img,
           const sub_image_info& infos ) const;

      void output_as_layer( const claw::graphic::image& img,
          const std::vector<sub_image_info>& infos ) const;

      void get_sub_images_info( const claw::graphic::image& img,
        std::vector<sub_image_info>& infos,
        const std::string& name ) const;

      void factorize( unsigned int n, std::vector<unsigned int>& vals,
          unsigned int max ) const;

      void extract_file_name( const std::string& file_name,
            std::string& name ) const;

      void format_file_name( std::string& str ) const;

    private:
      /** \brief The files to compile. */
      std::set<std::string> m_files;

      /** \brief Tell if we should quit immediatly. */
      bool m_quit;

      /** \brief The position of the picture in the layer. */
      claw::math::coordinate_2d<unsigned int> m_pos;

      /** \brief Maximum size of the sub images. */
      claw::math::coordinate_2d<unsigned int> m_max;

      /** \brief Tell if we must output the declaration of a decoration
    layer. */
      bool m_output_as_layer;

      /** \brief Path to the output folder. */
      std::string m_output_folder;

      /** \brief The prefix of the generated images files. */
      std::string m_file_prefix;

      /** \brief The type of the ouput files. */
      std::string m_output_type;

    }; // class application
  } // namespace ic
} // namespace ptb

#endif // __PTB_IC_APPLICATION_HPP__
