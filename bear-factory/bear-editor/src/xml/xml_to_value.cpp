/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::xml_to_value class.
 * \author Julien Jorge
 */
#include "bf/xml/xml_to_value.hpp"

#include "bf/image_pool.hpp"
#include "bf/workspace_environment.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/xml/reader_tool.hpp"

#include <claw/logger/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the bitmap_rendering_attributes of a node.
 * \param att (out) The attributes we have read.
 * \param node The node from which we read the value.
 */
void
bf::xml::bitmap_rendering_attributes_xml_to_value::load_rendering_attributes
( bitmap_rendering_attributes& att, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node != NULL );

  att.set_auto_size
    ( xml::reader_tool::read_bool_opt
      (node, wxT("auto_size"), att.get_auto_size()) );
  att.set_width
    ( xml::reader_tool::read_uint_opt(node, wxT("width"), att.width()) );
  att.set_height
    ( xml::reader_tool::read_uint_opt(node, wxT("height"), att.height()) );
  att.mirror
    ( xml::reader_tool::read_trinary_logic_opt
      (node, wxT("mirror"), att.get_mirrored_status()));
  att.flip
    ( xml::reader_tool::read_trinary_logic_opt
      (node, wxT("flip"), att.get_flipped_status()));
  att.get_color().set_opacity
    ( xml::reader_tool::read_real_opt
      (node, wxT("opacity"), att.get_color().get_opacity()) );
  att.set_angle
    ( xml::reader_tool::read_real_opt( node, wxT("angle"), att.get_angle()) );

  double r = xml::reader_tool::read_real_opt
    ( node, wxT("red_intensity"), att.get_color().get_red_intensity() );
  double g = xml::reader_tool::read_real_opt
    ( node, wxT("green_intensity"), att.get_color().get_green_intensity() );
  double b = xml::reader_tool::read_real_opt
    ( node, wxT("blue_intensity"), att.get_color().get_blue_intensity() );

  att.get_color().set_intensity( r, g, b );
} // bitmap_rendering_attributes_xml_to_value::load_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the value from a xml value node.
 * \param spr (out) The sprite we have read.
 * \param node The node from which we read the value.
 * \param env The workspace environment to use.
 */
void bf::xml::xml_to_value<bf::sprite>::operator()
  ( sprite& spr, const wxXmlNode* node, workspace_environment& env ) const
{
  CLAW_PRECOND( node != NULL );

  spr.set_image_name( xml::reader_tool::read_string(node, wxT("image")) );
  const std::string spritepos =
    xml::reader_tool::read_string_opt(node, wxT("spritepos"), std::string());

  if ( spritepos.empty() )
    {
      spr.set_left( xml::reader_tool::read_uint(node, wxT("x")) );
      spr.set_top( xml::reader_tool::read_uint(node, wxT("y")) );
      spr.set_clip_width
        ( xml::reader_tool::read_uint(node, wxT("clip_width")) );
      spr.set_clip_height
        ( xml::reader_tool::read_uint(node, wxT("clip_height")) );
      spr.set_spritepos_entry
        ( wx_to_std_string
          ( env.get_image_pool().find_spritepos_name_from_size
            ( std_to_wx_string(spr.get_image_name()),
              spr.get_clip_rectangle() )) );
    }
  else
    {
      spr.set_clip_rectangle
        ( env.get_image_pool().get_spritepos_rectangle
          ( std_to_wx_string(spr.get_image_name()),
            std_to_wx_string(spritepos) ) );
      spr.set_spritepos_entry( spritepos );
    }

  load_rendering_attributes(spr, node);

  if ( spr.get_auto_size() )
    {
      spr.set_width( spr.get_clip_width() );
      spr.set_height( spr.get_clip_height() );
    }
  else if ( (spr.width() == spr.get_clip_width())
            && (spr.height() == spr.get_clip_height()) )
    spr.set_auto_size(true);
} // xml_to_value::operator()() [sprite]

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the reader can process a node with a given name.
 * \param node_name The name of the node.
 */
bool bf::xml::xml_to_value<bf::animation>::supported_node
( const wxString& node_name )
{
  return node_name == wxT("animation");
} // xml_to_value::supported_node() [animation]

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the value from a xml value node.
 * \param anim (out) The animation we have read.
 * \param node The node from which we read the value.
 * \param env The workspace environment to use.
 */
void bf::xml::xml_to_value<bf::animation>::operator()
  ( animation& anim, const wxXmlNode* node, workspace_environment& env ) const
{
  CLAW_PRECOND( node != NULL );

  anim.set_loops( xml::reader_tool::read_uint(node, wxT("loops")) );
  anim.set_first_index
    ( xml::reader_tool::read_uint(node, wxT("first_index")) );
  anim.set_last_index
  ( xml::reader_tool::read_uint(node, wxT("last_index")) );
  anim.set_loop_back
  ( xml::reader_tool::read_bool_opt(node, wxT("loop_back"), false));

  load_frames(anim, node->GetChildren(), env);

  load_rendering_attributes(anim, node);

  if ( anim.get_auto_size() )
    {
      anim.set_width( anim.get_max_size().x );
      anim.set_height( anim.get_max_size().y );
    }
  else if ( anim.get_size() == anim.get_max_size() )
    anim.set_auto_size(true);
} // xml_to_value::operator()() [animation]

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the frames of an animation.
 * \param anim The animation in which we set a list of frames.
 * \param node The node to parse.
 * \param env The workspace environment to use.
 */
void bf::xml::xml_to_value<bf::animation>::load_frames
( animation& anim, const wxXmlNode* node, workspace_environment& env ) const
{
  for ( ; node!=NULL; node=node->GetNext() )
    if ( node->GetName() == wxT("frame") )
      load_frame(anim, node, env);
    else if ( node->GetName() != wxT("comment") )
      claw::logger << claw::log_warning << "Ignored node '"
                   << wx_to_std_string(node->GetName()) << "'" << std::endl;
} // bf::xml::xml_to_value<bf::animation>::load_frames()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a frame of an animation.
 * \param anim The animation in which we add the frame.
 * \param node The node to parse.
 * \param env The workspace environment to use.
 */
void bf::xml::xml_to_value<bf::animation>::load_frame
( animation& anim, const wxXmlNode* node, workspace_environment& env ) const
{
  CLAW_PRECOND( node->GetName() == wxT("frame") );

  wxString val;
  animation_frame frame;
  sprite spr;

  if ( !node->GetAttribute( wxT("duration"), &val ) )
    throw missing_property( "duration" );

  frame.set_duration
    ( xml::reader_tool::read_real_opt(node, wxT("duration"), 40) );

  wxXmlNode* children = node->GetChildren();
  if ( children != NULL )
    {
      if ( children->GetName() == wxT("sprite") )
        {
          xml::xml_to_value<sprite> xml_conv;
          xml_conv(spr, children, env);
          frame.set_sprite(spr);
          anim.add_frame() = frame;
        }
      else if ( node->GetName() != wxT("comment") )
        claw::logger << claw::log_warning << "Ignored node '"
                     << wx_to_std_string(children->GetName())
                     << "'" << std::endl;
    }
  else
    throw missing_node("sprite");
} // bf::xml::xml_to_value<bf::animation>::load_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the reader can process a node with a given name.
 * \param node_name The name of the node.
 */
bool bf::xml::xml_to_value<bf::animation_file_type>::supported_node
( const wxString& node_name )
{
  return node_name == wxT("animation_file");
} // xml_to_value::supported_node() [animation_file_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the value from a xml value node.
 * \param anim (out) The animation we have read.
 * \param node The node from which we read the value.
 * \param env The workspace environment to use.
 */
void bf::xml::xml_to_value<bf::animation_file_type>::operator()
  ( animation_file_type& anim, const wxXmlNode* node,
    workspace_environment& env ) const
{
  CLAW_PRECOND( node != NULL );

  wxString path;

  if ( !node->GetAttribute( wxT("path"), &path ) )
    throw missing_property("path");

  anim.set_path( wx_to_std_string(path), env );

  load_rendering_attributes(anim, node);

  const claw::math::coordinate_2d<unsigned int> size
    ( anim.get_animation().get_max_size() );

  if ( anim.get_auto_size() )
    {
      anim.set_width( size.x );
      anim.set_height( size.y );
    }
  else if ( anim.get_size() == size )
    anim.set_auto_size(true);
} // xml_to_value::operator()() [animation_file_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the reader can process a node with a given name.
 * \param node_name The name of the node.
 */
bool bf::xml::xml_to_value<bf::any_animation>::supported_node
( const wxString& node_name )
{
  return xml_to_value<animation_file_type>::supported_node(node_name)
    || xml_to_value<animation>::supported_node(node_name);
} // xml_to_value::supported_node() [any_animation]

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the value from a xml value node.
 * \param anim (out) The animation we have read.
 * \param node The node from which we read the value.
 * \param env The workspace environment tu use.
 */
void bf::xml::xml_to_value<bf::any_animation>::operator()
  ( any_animation& anim, const wxXmlNode* node,
    workspace_environment& env ) const
{
  wxString name = node->GetName();

  if ( xml_to_value<animation_file_type>::supported_node(name) )
    {
      animation_file_type data;
      xml_to_value<animation_file_type> reader;
      reader(data, node, env);
      anim.set_animation_file(data);
    }
  else if ( xml_to_value<animation>::supported_node(name) )
    {
      animation data;
      xml_to_value<animation> reader;
      reader(data, node, env);
      anim.set_animation(data);
    }
  else
    throw bad_node( wx_to_std_string(name) );
} // xml_to_value::operator()() [any_animation]

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the value from a xml value node.
 * \param v (out) The sample we have read.
 * \param node The node from which we read the value.
 */
void bf::xml::xml_to_value<bf::sample>::operator()
  ( sample& v, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node != NULL );

  wxString path;

  if ( !node->GetAttribute( wxT("path"), &path ) )
    throw missing_property("path");

  v.set_path( wx_to_std_string(path) );

  v.set_loops( xml::reader_tool::read_uint_opt(node, wxT("loops"), 1) );
  v.set_volume( xml::reader_tool::read_real_opt(node, wxT("volume"), 1) );
} // xml_to_value::operator()() [sample]

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the value from a xml value node.
 * \param v (out) The font we have read.
 * \param node The node from which we read the value.
 */
void bf::xml::xml_to_value<bf::font>::operator()
  ( font& v, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node != NULL );

  wxString path;

  if ( !node->GetAttribute( wxT("path"), &path ) )
    throw missing_property("path");

  const font default_value;

  v.set_font_name( wx_to_std_string(path) );
  v.set_size
    ( xml::reader_tool::read_real_opt
      ( node, wxT("size"), default_value.get_size() ) );
} // xml_to_value::operator()() [font]

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the value from a xml value node.
 * \param v (out) The color we have read.
 * \param node The node from which we read the value.
 */
void bf::xml::xml_to_value<bf::color>::operator()
  ( color& v, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node != NULL );
  
  const color default_value;
  
  v.set_opacity
    ( xml::reader_tool::read_real_opt
      ( node, wxT("opacity"), default_value.get_opacity() ) );
  
  double red =
    xml::reader_tool::read_real_opt
    ( node, wxT("red_intensity"), default_value.get_red_intensity() );
  
  double green =
    xml::reader_tool::read_real_opt
    ( node, wxT("green_intensity"), default_value.get_green_intensity() );

  double blue =
    xml::reader_tool::read_real_opt
    ( node, wxT("blue_intensity"), default_value.get_blue_intensity() );

  v.set_intensity(red, green, blue);
} // xml_to_value::operator()() [color]

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the value from a xml value node.
 * \param v (out) The easing function we have read.
 * \param node The node from which we read the value.
 */
void bf::xml::xml_to_value<bf::easing_type>::operator()
  ( easing_type& v, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node != NULL );
  
  bear::easing e;

  e.set_function
    ( bear::easing::function::from_string
      ( xml::reader_tool::read_string_opt
        ( node, wxT("function"),
          bear::easing::function::to_string( e.get_function() ) ) ) );
  
  e.set_direction
    ( bear::easing::direction::from_string
      ( xml::reader_tool::read_string_opt
        ( node, wxT("direction"),
          bear::easing::direction::to_string( e.get_direction() ) ) ) );
  
  v.set_value( e );
} // xml_to_value::operator()() [easing_type]
