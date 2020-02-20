/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
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
#include "bf/main_frame.hpp"
#include "bf/path_configuration.hpp"
#include "bf/version.hpp"

#include <wx/tooltip.h>

#include <claw/exception.hpp>
#include <claw/logger.hpp>

#include <limits>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/path.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Configure the program.
 */
void bf::animation_editor::configure()
{
  config_frame dlg(NULL);

  if ( dlg.ShowModal() == wxID_OK )
    {
      // TO DO :
      // Call update_image_pool() for each main_frame
      ;
    }
} // animation_editor::configure()

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
      std::string w = 
        path_configuration::get_instance().search_workspace
        ( wx_to_std_string(path) );
      
      if ( ! w.empty() )
        {
          workspace_environment env(w);
          
          animation_file_xml_reader reader;
          animation anim( reader.load( doc.GetRoot(), env ) );
          compile_animation(anim, path);
        }
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
      std::string w = 
        path_configuration::get_instance().search_workspace
        ( wx_to_std_string(path) );
      workspace_environment env;
      
      if ( ! w.empty() )
        env = workspace_environment( w );

      animation_file_xml_reader reader;
      anim = reader.load( doc.GetRoot(), env );
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
      std::string w = 
        path_configuration::get_instance().search_workspace
        ( wx_to_std_string(path) );
      workspace_environment env;
      
      if ( ! w.empty() )
        env = workspace_environment( w );

      compiled_file cf(f);
      cf << BF_MAJOR_VERSION << BF_MINOR_VERSION << BF_RELEASE_NUMBER;

      compilation_context context
        ( std::numeric_limits<unsigned int>::max(), env );
      anim.compile(cf, context);
    }
  else
    throw claw::exception("Can't open the animation file.");
} // animation_editor::compile_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the application.
 * \param default_env Default workspace_environment.
 */
bool bf::animation_editor::do_init_app
( const workspace_environment& default_env )
{
  bool result = false;

  init_config();

  main_frame* frame = NULL;

  if (argc > 1)
    for (int i=1; i<argc; ++i)
      {
        std::string w =
          path_configuration::get_instance().search_workspace
          ( wx_to_std_string( argv[i] ) );
        
        workspace_environment env(w);

        if ( ! w.empty() )
          {
            frame = new main_frame(env);
            frame->load_animation( argv[i] );
            frame->Show();
            result = true;
          }
        else
          std::cout << "Error. No workspace is available for animation " 
                    << wx_to_std_string( argv[i] ) << std::endl;
      }
  else
    {
      frame = new main_frame( default_env );
      frame->SetSize( m_config.main_rect );
      frame->Show();
      result = true;
    }

  return result;
} // animation_editor::do_init_app()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load and apply the configuration.
 */
void bf::animation_editor::init_config()
{
  m_config.load();
} // animation_editor::init_config()
