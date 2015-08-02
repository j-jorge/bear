/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::level_file_xml_reader class.
 * \author Julien Jorge
 */
#include "bf/level_file_xml_reader.hpp"

#include "bf/gui_level.hpp"
#include "bf/item_class_pool.hpp"
#include "bf/layer.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/xml/exception.hpp"
#include "bf/xml/item_instance_node.hpp"
#include "bf/xml/reader_tool.hpp"

#include <claw/logger.hpp>

#include "bf/image_pool.hpp"
#include "bf/workspace_environment.hpp"
void migrate( bf::level& lvl );
const bf::image_pool* g_image_pool( NULL );

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a level.
 * \param file_path The path to the level file.
 * \param env The workspace environment used.
 */
bf::gui_level* bf::level_file_xml_reader::load
( const wxString& file_path, workspace_environment& env ) const
{
  g_image_pool = &env.get_image_pool();
  wxXmlDocument doc;

  if ( !doc.Load(file_path) )
    throw std::ios_base::failure
      ( "Cannot load the XML file '" + wx_to_std_string(file_path) + "'" );

  wxXmlNode* node = doc.GetRoot();

  if ( node == NULL )
    throw xml::missing_node("level");

  return load_level( node, env );
} // level_file_xml_reader::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a node of type "level".
 * \param node The node to parse.
 * \param env The workspace environment used.
 */
bf::gui_level* bf::level_file_xml_reader::load_level
( const wxXmlNode* node, workspace_environment& env ) const
{
  if ( node->GetName() != wxT("level") )
    throw xml::bad_node( wx_to_std_string(node->GetName()) );

  unsigned int width, height;
  wxString music, name;

  width = xml::reader_tool::read_uint(node, wxT("width"));
  height = xml::reader_tool::read_uint(node, wxT("height"));

  music = node->GetPropVal( wxT("music"), wxEmptyString );
  name = node->GetPropVal( wxT("name"), wxEmptyString );

  gui_level* lvl = NULL;

  try
    {
      lvl = new gui_level
        ( wx_to_std_string(name), width, height, wx_to_std_string(music) );
      load_layers( *lvl, node->GetChildren(), env );

      migrate( *lvl );
    }
  catch( std::exception& e )
    {
      delete lvl;
      throw;
    }

  return lvl;
} // level_file_xml_reader::load_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the layers of the level.
 * \param pool The pool of item classes in which we take the item classes.
 * \param lvl The level in which we store the layers.
 * \param node The node to parse.
 * \param env The workspace environment used.
 */
void bf::level_file_xml_reader::load_layers
( gui_level& lvl, const wxXmlNode* node, workspace_environment& env ) const
{
  for ( node=xml::reader_tool::skip_comments(node); node!=NULL;
        node=xml::reader_tool::skip_comments(node->GetNext()) )
    if ( node->GetName() == wxT("layer") )
      load_layer(lvl, node, env);
    else
      claw::logger << claw::log_warning << "Ignored node '"
                   << wx_to_std_string(node->GetName()) << "'" << std::endl;
} // level_file_xml_reader::load_layers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load one layer.
 * \param lvl The level in which we store the layer.
 * \param node The node to parse.
 * \param env The workspace environment used.
 */
void bf::level_file_xml_reader::load_layer
( gui_level& lvl, const wxXmlNode* node, workspace_environment& env ) const
{
  CLAW_PRECOND( node->GetName() == wxT("layer") );

  bool fit_level;
  unsigned int width, height;
  wxString val;
  wxString name;
  std::string tag;

  width = xml::reader_tool::read_uint(node, wxT("width"));
  height = xml::reader_tool::read_uint(node, wxT("height"));
  fit_level = ((width == lvl.get_width()) && (height == lvl.get_height()))
    || xml::reader_tool::read_bool_opt( node, wxT("fit_level"), false );
  tag = xml::reader_tool::read_string_opt( node, wxT("tag"), "" );

  if ( !node->GetPropVal( wxT("class_name"), &val ) )
    throw xml::missing_property( "class_name" );

  node->GetPropVal( wxT("name"), &name );

  layer& lay =
    lvl.add_layer
    (wx_to_std_string(val), wx_to_std_string(name), fit_level, width, height );
  
  if ( ! tag.empty() )
    lay.set_tag(tag);

  load_layer_content( lay, node->GetChildren(), env );
} // level_file_xml_reader::load_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the content of a layer.
 * \param lay The layer in which we store the instances.
 * \param node The node to parse.
 * \param env The workspace environment used.
 */
void bf::level_file_xml_reader::load_layer_content
( layer& lay, const wxXmlNode* node, workspace_environment& env ) const
{
  node=xml::reader_tool::skip_comments(node);

  if ( node != NULL )
    {
      if ( node->GetName() == wxT("items") )
        load_items( lay, node->GetChildren(), env );

      node=xml::reader_tool::skip_comments(node->GetNext());

      if ( node != NULL )
        if ( node->GetName() == wxT("priority") )
          load_priorities( lay, node->GetChildren() );
    }
} // level_file_xml_reader::load_layer_content()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the items of a layer.
 * \param lay The layer in which we store the instances.
 * \param node The node to parse.
 * \param env The workspace environment used.
 */
void bf::level_file_xml_reader::load_items
( layer& lay, const wxXmlNode* node, workspace_environment& env ) const
{
  for ( node=xml::reader_tool::skip_comments(node); node!=NULL;
        node=xml::reader_tool::skip_comments(node->GetNext()) )
    if ( node->GetName() == wxT("item") )
      load_item(lay, node, env);
    else
      claw::logger << claw::log_warning << "Ignored node '"
                   << wx_to_std_string(node->GetName()) << "'" << std::endl;
} // level_file_xml_reader::load_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the priority of the items.
 * \param lay The layer in which we take the items.
 * \param node The node to parse.
 */
void bf::level_file_xml_reader::load_priorities
( layer& lay, const wxXmlNode* node ) const
{
  for ( node=xml::reader_tool::skip_comments(node); node!=NULL;
        node=xml::reader_tool::skip_comments(node->GetNext()) )
    if ( node->GetName() == wxT("ref") )
      {
        const std::string id = xml::reader_tool::read_string(node, wxT("id"));
        layer::item_iterator item = lay.find_item_by_id( id );

        if ( item != lay.item_end() )
          lay.prioritize( &(*item) );
        else
          claw::logger << claw::log_warning << "Can't prioritize unknown item '"
                       << id << "'" << std::endl;
      }
    else
      claw::logger << claw::log_warning << "Ignored node '"
                   << wx_to_std_string(node->GetName()) << "'" << std::endl;
} // level_file_xml_reader::load_priorities()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an item of a layer.
 * \param lay The layer in which we store the item.
 * \param node The node to parse.
 * \param env The workspace environment used.
 */
void bf::level_file_xml_reader::load_item
( layer& lay, const wxXmlNode* node, workspace_environment& env ) const
{
  CLAW_PRECOND( node->GetName() == wxT("item") );

  xml::item_instance_node item_node(env);
  item_instance* item = item_node.read( node );

  if ( item != NULL )
    lay.add_item(item);
} // level_file_xml_reader::load_item()

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

std::string random( const std::string& a, const std::string& b )
{
  static bool init( true );
  if ( init )
    {
      srand( time( NULL ) );
      init = false;
    }
  
  if ( rand() < RAND_MAX / 2 )
    return a;
  else
    return b;
}

void migrate_slope_sprite( const std::string& theme, bf::sprite& s )
{
  const std::string name( s.get_image_name() );

  std::vector< std::string > parts;
  std::string slope_type
    ( boost::algorithm::split( parts, name, boost::is_any_of( "/" ) )[ 2 ] );
  std::string slope_name
    ( boost::algorithm::split
      ( parts,
        boost::algorithm::split
        ( parts, name, boost::is_any_of( "/" ) )[ 3 ],
        boost::is_any_of( "." ) )[ 0 ] );

  if ( slope_type == "ground" )
    return;
  
  if ( slope_type == "bump" )
    {
      s.set_image_name( "gfx/" + theme + "/ground/ground-1.png" );
      s.set_spritepos_entry( slope_name );
    }
  else if ( slope_type == "gentle-slope" )
    {
      s.set_image_name( "gfx/" + theme + "/ground/ground-1.png" );

      if ( slope_name == "gentle-slope-3*" )
        s.set_spritepos_entry
          ( random( "gentle-slope-3", "gentle-slope-3-alt" ) );
      else if ( slope_name == "gentle-slope-5*" )
        s.set_spritepos_entry
          ( random( "gentle-slope-5", "gentle-slope-5-alt" ) );
      else        
        s.set_spritepos_entry( slope_name );
    }
  else if ( slope_type == "sin-slope" )
    {
      s.set_image_name( "gfx/" + theme + "/ground/ground-2.png" );

      if ( slope_name == "sin-slope-4*" )
        s.set_spritepos_entry
          ( random( "sin-slope-4", "sin-slope-4-alt" ) );
      else        
        s.set_spritepos_entry( slope_name );
    }
  else if ( slope_type == "steep-slope" )
    {
      s.set_image_name( "gfx/" + theme + "/ground/ground-3.png" );

      if ( slope_name == "steep-slope-4*" )
        s.set_spritepos_entry
          ( random( "steep-slope-4", "steep-slope-4-alt" ) );
      else if ( slope_name == "steep-slope-5*" )
        s.set_spritepos_entry
          ( random( "steep-slope-5", "steep-slope-5-alt" ) );
      else        
        s.set_spritepos_entry( slope_name );
    }
  else
    std::cout << "Ignoring slope " << slope_type << ' ' << slope_name << '\n';
}

void migrate_wall_fill_sprite( const std::string& theme, bf::sprite& s )
{
  const std::string name( s.get_image_name() );

  std::vector< std::string > parts;
  std::string type
    ( boost::algorithm::split( parts, name, boost::is_any_of( "/" ) )[ 2 ] );

  if ( type == theme )
    return;

  if ( type == "wall-fill-*.png" )
    {
      s.set_image_name( "gfx/wall-fill/" + theme + "/wall-fill*.png" );
      s.set_spritepos_entry( "wall-fill" );
    }
  else if ( boost::starts_with( type, "wall-fill-" ) )
    {
      s.set_image_name( "gfx/wall-fill/" + theme + "/" + type );
      s.set_spritepos_entry( "wall-fill" );
    }
  else
    std::cout << "Ignoring wall fill " << type << '\n';
}

void migrate_rail_sprite( const std::string& theme, bf::sprite& s )
{
  const std::string name( s.get_image_name() );

  std::vector< std::string > parts;
  std::string type
    ( boost::algorithm::split( parts, name, boost::is_any_of( "/" ) )[ 2 ] );

  if ( type == "ground" )
    return;

  if ( ( type == "rail.png" ) || ( type == "rail-end.png" )
       || ( type == "broken-rail.png" ) )
      s.set_image_name( "gfx/" + theme + "/ground/ground-2.png" );
  else
    std::cout << "Ignoring rail " << type << '\n';
}

void migrate_sprite
( const std::string& theme, bf::item_instance& item, const std::string& field )
{
  if ( !item.has_value( field ) )
    return;
  
  bf::sprite s;
  item.get_value( field, s );

  const std::string name( s.get_image_name() );
  
  if ( boost::starts_with( name, "gfx/slope/" ) )
    migrate_slope_sprite( theme, s );
  else if ( boost::starts_with( name, "gfx/rail/" ) )
    migrate_rail_sprite( theme, s );
  else if ( boost::starts_with( name, "gfx/wall-fill/" ) )
    migrate_wall_fill_sprite( theme, s );

  if ( !s.get_spritepos_entry().empty() )
    s.set_clip_rectangle
      ( g_image_pool->get_spritepos_rectangle
        ( bf::std_to_wx_string( s.get_image_name() ),
          bf::std_to_wx_string( s.get_spritepos_entry() ) ) );
  
  item.set_value( field, s );
}

void migrate( const std::string& theme, bf::item_instance& item )
{
  const bf::item_class& c( item.get_class() );
  
  std::list< std::string > fields;
  c.get_field_names_in_hierarchy( fields );

  for ( std::list< std::string >::const_iterator it( fields.begin() );
        it != fields.end(); ++it )
    if ( c.get_field( *it ).get_field_type()
         == bf::type_field::sprite_field_type )
      migrate_sprite( theme, item, *it );
}

void migrate( const std::string& theme, bf::layer& layer )
{
  for ( bf::layer::item_iterator it( layer.item_begin_no_filter() );
        it != layer.item_end_no_filter();
        ++it )
    migrate( theme, *it );
}

void migrate( const std::string& theme, bf::level& lvl )
{
  for ( std::size_t i( 0 ); i != lvl.layers_count(); ++i )
    migrate( theme, lvl.get_layer( i ) );
}

std::string get_theme( const std::string& m )
{
  const std::string::size_type slash( m.find_last_of( '/' ) );

  if ( slash == std::string::npos )
    return "";
  
  const std::string::size_type dot( m.find_first_of( '.', slash ) );

  if ( dot == std::string::npos )
    return "";

  return m.substr( slash + 1, dot - slash - 1 );
}

void migrate( bf::level& lvl )
{
  const std::string theme( get_theme( lvl.get_music() ) );

  if ( theme == "aquatic" )
    migrate( theme, lvl );
  else
    std::cout << "Unknown theme '" << theme << "''\n";
}

