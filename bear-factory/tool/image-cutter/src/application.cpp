/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the ptb::ic::application class.
 * \author Julien Jorge
 */
#include "application.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <claw/graphic/png.hpp>
#include <claw/jpeg.hpp>
#include <claw/targa.hpp>
#include <claw/graphic/bitmap.hpp>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param argc Number of program arguments.
 * \param argv Program arguments.
 */
ptb::ic::application::application( int& argc, char** &argv )
  : claw::application(argc, argv), m_quit(false), m_pos(0, 0),
    m_output_as_layer(false), m_output_folder("./"), m_output_type("png")
{
  check_arguments( argc, argv );
} // application::application()

/*----------------------------------------------------------------------------*/
/**
 * \brief Run the application : split the image.
 */
int ptb::ic::application::run()
{
  int result = 0;

  if (!m_quit)
    {
      if ( m_files.empty() )
        {
          std::cerr << "No file to split." << std::endl;
          result = -1;
        }
      else
        {
          std::set<std::string>::const_iterator it;

          for (it=m_files.begin(); it!=m_files.end(); ++it)
            process_file(*it);
        }
    }

  return result;
} // application::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Print program usage.
 */
void ptb::ic::application::help() const
{
  m_arguments.help("files...");
} // application::help()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check program arguments.
 * \param argc Number of program arguments.
 * \param argv Program arguments.
 */
void ptb::ic::application::check_arguments( int& argc, char** &argv )
{
  m_arguments.add("-l", "--output-as-layer",
                  "Output the declaration for a decoration layer.", true);
  m_arguments.add("-d", "--output-folder",
                  "Path to the directory where we save the images.", true);
  m_arguments.add("-o", "--output-format",
                  "The type of the output files. Default is png.", true,
                  "{tga, png, bmp, jpg}");
  m_arguments.add_short("-x", "X-coordinate of the picture in the level.",
                        true);
  m_arguments.add_short("-y", "Y-coordinate of the picture in the level.",
                        true);
  m_arguments.add
    ("-f", "--file-prefix",
     "The prefix of the name of the files (sub path included).", true);
  m_arguments.add("-X", "--max-x",
                  "The maximum size of the sub images on the X-coordinates",
                  true);
  m_arguments.add("-Y", "--max-y",
                  "The maximum size of the sub images on the Y-coordinates",
                  true);
  m_arguments.add("-h", "--help", "Print this message and exit.", true);

  m_arguments.parse( argc, argv );

  if ( m_arguments.get_bool("-h") )
    {
      help();
      m_quit = true;
    }

  store_files(argc, argv);

  m_output_as_layer = m_arguments.get_bool("--output-as-layer");

  if ( m_arguments.has_value("--output-folder") )
    m_output_folder = m_arguments.get_string("--output-folder") + "/";

  if ( m_files.size() == 1 )
    {
      if ( m_arguments.has_value("-x") )
        m_pos.x = m_arguments.get_integer("-x");

      if ( m_arguments.has_value("-y") )
        m_pos.y = m_arguments.get_integer("-y");

      if ( m_arguments.has_value("--file-prefix") )
        m_file_prefix = m_arguments.get_string("--file-prefix");
      else if ( m_arguments.has_value("--name-prefix") )
        m_file_prefix = m_arguments.get_string("--name-prefix");

      if ( m_arguments.has_value("-X") )
        if ( m_arguments.get_integer("-X") == 0 )
          {
            std::cerr << "--max-x parameter must be greater than zero"
                      << std::endl;
            m_quit = true;
          }

      if ( m_arguments.has_value("-Y") )
        if ( m_arguments.get_integer("-Y") == 0 )
          {
            std::cerr << "--max-y parameter must be greater than zero"
                      << std::endl;
            m_quit = true;
          }

      if ( m_arguments.has_value("-o") )
        {
          std::string format = m_arguments.get_string("-o");

          if ( (format != "png") && (format != "tga") && (format != "bmp")
               && (format != "png") )
            {
              std::cerr << "--o parameter must be one of {png, tga, bmp, jpg}. "
                        << "Switching back to png." << std::endl;
              m_quit = true;
            }
          else
            m_output_type = format;
        }
    }

} // application::application()

/*----------------------------------------------------------------------------*/
/**
 * \brief Store the names of the valid files to compile.
 * \param argc Number of program arguments.
 * \param argv Program arguments.
 */
void ptb::ic::application::store_files( int& argc, char** &argv )
{
  if ( argc == 0 )
    {
      if ( !std::cin.eof() )
        m_files.insert("-");
    }
  else
    for (int i=0; i!=argc; ++i)
      if ( std::string(argv[i]) == "-" )
        m_files.insert(argv[i]);
      else
        {
          std::ifstream f(argv[i]);

          if (f)
            {
              m_files.insert(argv[i]);
              f.close();
            }
          else
            std::cerr << argv[i] << ": Can't open file." << std::endl;
        }
} // application::store_files()

/*----------------------------------------------------------------------------*/
/**
 * \brief Process a file.
 * \param name The name of the file to process.
 */
void ptb::ic::application::process_file( const std::string& name )
{
  if ( name == "-" )
    process_file( std::cin, "stdin" );
  else
    {
      std::ifstream f(name.c_str());

      if (f)
        {
          process_file(f, name);
          f.close();
        }
      else
        std::cerr << "Can't open file for reading: " << name << std::endl;
    }
} // application::process_file() [std::string]

/*----------------------------------------------------------------------------*/
/**
 * \brief Process a file.
 * \param is The file to process.
 * \param name The name of the file.
 */
void ptb::ic::application::process_file( std::istream& is,
                                         const std::string& name )
{
  claw::graphic::image* img = load_image(is);

  if (img)
    {
      if ( m_arguments.has_value("--max-x") )
        m_max.x = m_arguments.get_integer("--max-x");
      else
        m_max.x = img->width();

      if ( m_arguments.has_value("--max-y") )
        m_max.y = m_arguments.get_integer("--max-y");
      else
        m_max.y = img->height();

      split_image(*img, name);

      delete img;
    }
  else
    std::cerr << "Unhandled image format: " << name << std::endl;

} // application::process_file() [std::istream]

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an image from a stream.
 * \param is The stream to read from.
 */
claw::graphic::image* ptb::ic::application::load_image( std::istream& is ) const
{
  claw::graphic::image* result = NULL;

  try { result = new claw::graphic::image(is); }
  catch(...) { }

  return result;
} // application::load_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Cut the image in smallest images.
 * \param img The image to split.
 * \param name The name of the image.
 */
void ptb::ic::application::split_image( const claw::graphic::image& img,
                                        const std::string& name ) const
{
  std::vector<sub_image_info> infos;
  std::vector<sub_image_info>::const_iterator it;

  get_sub_images_info( img, infos, name );

  for (it=infos.begin(); it!=infos.end(); ++it)
    save_image( img, *it );

  if ( m_output_as_layer )
    output_as_layer( img, infos );
} // application::split_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save a part of the source image.
 * \param img The image to split.
 * \param infos Informations about the part to save.
 */
void ptb::ic::application::save_image( const claw::graphic::image& img,
                                       const sub_image_info& infos ) const
{
  std::string output_name = m_output_folder + infos.path;
  std::ofstream f( output_name.c_str() );

  if (f)
    {
      claw::math::coordinate_2d<int> pos( -infos.box.position.x,
                                          -infos.box.position.y );
      claw::graphic::image part( infos.box.width, infos.box.height );

      part.partial_copy( img, pos );

      if ( m_output_type == "tga" )
        claw::graphic::targa::writer( part, f, true );
      else if ( m_output_type == "png" )
        claw::graphic::png::writer( part, f );
      else if ( m_output_type == "jpg" )
        claw::graphic::jpeg::writer( part, f );
      else if ( m_output_type == "bmp" )
        claw::graphic::bitmap::writer( part, f );

      f.close();
    }
  else
    std::cout << "Can't open file for writing: " << output_name << std::endl;
} // application::save_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Ouptut a decoration layer declaration of the sub images.
 * \param img The image we have splitted.
 * \param infos Informations about the parts of the images.
 */
void ptb::ic::application::output_as_layer
( const claw::graphic::image& img,
  const std::vector<sub_image_info>& infos ) const
{
  std::cout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
  std::cout << "<level width='" << img.width() << "' height='"
            << img.height() << "'>\n";
  std::cout << "  <layer class_name='decoration_layer' width='" << img.width()
            << "' height='" << img.height() << "' fit_level='0'>\n\n";
  std::cout << "  <items>\n\n";

  for (unsigned int i=0; i!=infos.size(); ++i)
    std::cout << "    <item class_name='bear::decorative_item' fixed='true'>\n"
              << "      <fields>\n"
              << "        <field name='base_item.position.left'>\n"
              << "          <real value='" << infos[i].box.position.x + m_pos.x
              << "'/>\n"
              << "        </field>\n"
              << "        <field name='base_item.position.bottom'>\n"
              << "          <real value='"
              << img.height()
      - (infos[i].box.position.y + infos[i].box.height + m_pos.y)
              << "'/>\n"
              << "        </field>\n"
              << "        <field name='item_with_decoration.sprite'>\n"
              << "          <sprite image='" << infos[i].path << "' x='0' "
              << "y='0' clip_width='" << infos[i].box.width << "' clip_height='"
              << infos[i].box.height << "'/>\n"
              << "        </field>\n"
              << "      </fields>\n"
              << "    </item>\n\n";

  std::cout << "  </items>\n";
  std::cout << "  </layer><!-- decoration_layer -->\n"
            << "</level>\n" << std::endl;
} // application::output_as_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get informations on the sub images.
 * \param img The image we're splitting.
 * \param infos (out) The informations you want.
 * \param name The name of the image we're splitting.
 */
void ptb::ic::application::get_sub_images_info
( const claw::graphic::image& img, std::vector<sub_image_info>& infos,
  const std::string& name ) const
{
  std::vector<unsigned int> x_sizes, y_sizes;

  factorize( img.width(), x_sizes, m_max.x );
  factorize( img.height(), y_sizes, m_max.y );

  std::string file_prefix;

  if (m_file_prefix.empty())
    extract_file_name( name, file_prefix );
  else
    file_prefix = m_file_prefix;

  format_file_name( file_prefix );

  claw::math::coordinate_2d<unsigned int> pos(0, 0);
  unsigned int i=0;
  std::ostringstream oss;

  oss << y_sizes.size() * x_sizes.size() - 1;
  const unsigned int field_width = oss.str().length();

  oss << std::setfill('0');

  for (unsigned int y=0; y!=y_sizes.size(); ++y)
    {
      pos.x = 0;

      for (unsigned int x=0; x!=x_sizes.size(); ++x, ++i)
        {
          sub_image_info info;
          info.box.position = pos;
          info.box.width = x_sizes[x];
          info.box.height = y_sizes[y];

          oss.str("");
          oss << file_prefix << std::setw(field_width) << i;
          info.path = oss.str() + "." + m_output_type;

          infos.push_back( info );

          pos.x += x_sizes[x];
        }

      pos.y += y_sizes[y];
    }
} // application::get_sub_images_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get The decomposition of an integer by the sum of powers of two.
 * \param n The value to factorize.
 * \param vals (out) The calculated values.
 * \param max The upper bound value in the sum.
 */
void ptb::ic::application::factorize
( unsigned int n, std::vector<unsigned int>& vals, unsigned int max ) const
{
  CLAW_PRECOND( n != 0 );

  const unsigned int bound( ( n >= max ) ? max : n );

  unsigned int v;
  for ( v=1; (v < bound) && /* overflow */ (v != 0); v *= 2 ) { }

  if ( v > bound )
    v /= 2;

  if ( n - v < 32 )
    vals.push_back( n );
  else
    {
      vals.push_back( v );
      factorize( n - v, vals, max );
    }
} // application::factorize()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the path and the extension of a file.
 * \param file_name The path of the file.
 * \param name (out) The extracted name.
 */
void ptb::ic::application::extract_file_name
( const std::string& file_name, std::string& name ) const
{
  std::string::size_type first = file_name.find_last_of("/");

  if ( first == std::string::npos )
    first = 0;
  else
    ++first;

  std::string::size_type last = file_name.find_last_of(".");

  if ( (last == std::string::npos) || (last < first) )
    last = file_name.length();

  name = file_name.substr(first, last-first);
} // application::extract_file_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Replace invalid characters of a file name with an underscore.
 * \param str (in/out) The string to format.
 */
void ptb::ic::application::format_file_name( std::string& str ) const
{
  const std::string invalid = "\" ";

  std::string::size_type pos = str.find_first_of(invalid);

  while ( pos != std::string::npos )
    {
      str[pos] = '_';
      pos = str.find_first_of(invalid, pos+1);
    }
} // application::format_file_name()
