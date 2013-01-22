/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::animation_editor class.
 * \author Julien Jorge.
 */
#include "bf/animation_editor.hpp"

#include "bf/animation_file_xml_reader.hpp"
#include "bf/animation_file_xml_writer.hpp"
#include "bf/compilation_context.hpp"
#include "bf/config_frame.hpp"
#include "bf/image_pool.hpp"
#include "bf/main_frame.hpp"
#include "bf/path_configuration.hpp"
#include "bf/version.hpp"

#include <wx/tooltip.h>

#include <claw/exception.hpp>
#include <claw/logger.hpp>

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Configure the program.
 */
void bf::animation_editor::configure()
{
  config_frame dlg(NULL);

  if ( dlg.ShowModal() == wxID_OK )
    update_image_pool();
} // animation_editor::configure()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the image pool.
 */
void bf::animation_editor::update_image_pool() const
{
  image_pool::get_instance().clear();

  std::list<std::string>::const_iterator it;

  for ( it=path_configuration::get_instance().data_path.begin();
        it!=path_configuration::get_instance().data_path.end(); ++it )
    image_pool::get_instance().scan_directory(*it);
} // animation_editor::update_image_pool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the position and the size of the main frame.
 */
void bf::animation_editor::set_main_rect( const wxRect& r )
{
  m_config.main_rect = r;
  m_config.save();
} // animation_editor::set_main_rect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile an animation.
 * \param path The path to the animation file.
 */
void bf::animation_editor::compile( const wxString& path ) const
{
  wxXmlDocument doc;

  if ( doc.Load(path) )
    {
      animation_file_xml_reader reader;
      animation anim( reader.load( doc.GetRoot() ) );
      compile_animation(anim, path);
    }
  else
    throw claw::exception("Can't load XML file.");
} // animation_editor::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update an animation.
 * \param path The path to the animation file.
 */
void bf::animation_editor::update( const wxString& path ) const
{
  wxXmlDocument doc;
  animation anim;

  if ( doc.Load(path) )
    {
      animation_file_xml_reader reader;
      anim = reader.load( doc.GetRoot() );
    }
  else
    throw claw::exception("Can't load XML file.");

  animation_file_xml_writer writer;
  std::ofstream f( wx_to_std_string(path).c_str() );
  writer.save(f, anim);
} // animation_editor::update()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile an animation.
 * \param anim The animation to compile.
 * \param path The path to the animation file.
 * \return true if the compilation went ok.
 */
void bf::animation_editor::compile_animation
( const animation& anim, const wxString& path ) const
{
  std::string std_path( wx_to_std_string(path) );
  std::size_t pos = std_path.rfind(".anim");

  if ( pos != std::string::npos )
    std_path = std_path.substr(0, pos);

  std_path += ".canim";

  std::ofstream f( std_path.c_str() );

  if (f)
    {
      compiled_file cf(f);
      cf << BF_MAJOR_VERSION << BF_MINOR_VERSION << BF_RELEASE_NUMBER;

      compilation_context context( std::numeric_limits<unsigned int>::max() );
      anim.compile(cf, context);
    }
  else
    throw claw::exception("Can't open the animation file.");
} // animation_editor::compile_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the application.
 */
bool bf::animation_editor::do_init_app()
{
  init_config();
  update_image_pool();

  main_frame* frame = NULL;

  if (argc > 1)
    for (int i=1; i<argc; ++i)
      {
        frame = new main_frame();
        frame->load_animation( argv[i] );
        frame->Show();
      }
  else
    {
      frame = new main_frame();
      frame->SetSize( m_config.main_rect );
      frame->Show();
    }

  return true;
} // animation_editor::init_app()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load and apply the configuration.
 */
void bf::animation_editor::init_config()
{
  m_config.load();

  if ( path_configuration::get_instance().data_path.empty() )
    {
      config_frame dlg(NULL);
      dlg.ShowModal();
    }
} // animation_editor::init_config()
