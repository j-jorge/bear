/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::configuration class.
 * \author Julien Jorge
 */
#include "bf/configuration.hpp"

#include "bf/path_configuration.hpp"

#include <boost/filesystem/convenience.hpp>
#include <fstream>
#include <sstream>

/*----------------------------------------------------------------------------*/
const std::string bf::configuration::s_config_file_name = "model-editor.config";

const char bf::configuration::s_section_left = '[';
const char bf::configuration::s_section_right = ']';
const char bf::configuration::s_comment = '#';
const char bf::configuration::s_field_assign = '=';

const std::string bf::configuration::s_main_frame_section = "main_frame";
const std::string bf::configuration::s_model_frame_section = "model_frame";

const std::string bf::configuration::s_x_field = "x";
const std::string bf::configuration::s_y_field = "y";
const std::string bf::configuration::s_width_field = "width";
const std::string bf::configuration::s_height_field = "height";

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::configuration::configuration()
{
  load();
} // configuration::configuration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the configuration.
 */
void bf::configuration::load()
{
  if ( create_config_file() )
    {
      std::string path
        ( path_configuration::get_instance().get_config_directory()
          + s_config_file_name );

      std::ifstream f( path.c_str() );

      if (f)
        {
          claw::configuration_file config(f);

          main_rect = read_rect( config, s_main_frame_section );

          default_model_window_rect =
            read_rect( config, s_model_frame_section );
        }
    }
} // configuration::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the configuration.
 */
void bf::configuration::save() const
{
  if ( create_config_file() )
    {
      std::string path
        ( path_configuration::get_instance().get_config_directory()
          + s_config_file_name );

      std::ofstream f( path.c_str() );

      if (f)
        {
          f << '\n' << s_section_left << s_main_frame_section << s_section_right
            << '\n' << s_comment << " Position and size of the main window\n";
          write_rect( f, main_rect );
          f << '\n';

          f << s_section_left << s_model_frame_section
            << s_section_right << '\n'
            << s_comment << " Size of the model frame\n";
          write_rect( f, default_model_window_rect );
          f << '\n';
        }
    }
} // configuration::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the configuration file, if it does not exists.
 * \return true if the file already exists or if it has been created.
 */
bool bf::configuration::create_config_file() const
{
  bool result = false;

  const boost::filesystem::path path
    ( path_configuration::get_instance().get_config_directory()
      + s_config_file_name );

  if ( !boost::filesystem::exists( path ) )
    {
      std::ofstream f( path.string().c_str() );
      f << s_comment << " Configuration file for Bear Factory - Model editor\n";
    }

  if ( boost::filesystem::exists( path ) )
    result = !boost::filesystem::is_directory( path );

  return result;
} // configuration::create_config_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a wxRect from a configuration file.
 * \param config The configuration file.
 * \param section The name of the section in which we get the rectangle.
 */
wxRect bf::configuration::read_rect
( const claw::configuration_file& config, const std::string& section ) const
{
  wxRect result( wxDefaultPosition, wxDefaultSize );

  std::istringstream iss
    ( config(section, s_x_field) + ' ' + config(section, s_y_field) + ' ' +
      config(section, s_width_field) + ' ' + config(section, s_height_field) );

  iss >> result.x >> result.y >> result.width >> result.height;

  return result;
} // configuration::read_rect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a wxRect in a file.
 * \param f The file to write in.
 * \param r The rectangle to write.
 */
void bf::configuration::write_rect( std::ostream& f, const wxRect& r ) const
{
  f << s_x_field << ' ' << s_field_assign << ' ' << r.x << '\n'
    << s_y_field << ' ' << s_field_assign << ' ' << r.y << '\n'
    << s_width_field << ' ' << s_field_assign << ' ' << r.width << '\n'
    << s_height_field << ' ' << s_field_assign << ' ' << r.height << '\n';
} // configuration::write_rect()

