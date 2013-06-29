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

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a level.
 * \param pool The pool of item classes in which we take the item classes.
 * \param file_path The path to the level file.
 */
bf::gui_level* bf::level_file_xml_reader::load
( const item_class_pool& pool, const wxString& file_path ) const
{
  wxXmlDocument doc;

  if ( !doc.Load(file_path) )
    throw std::ios_base::failure
      ( "Cannot load the XML file '" + wx_to_std_string(file_path) + "'" );

  wxXmlNode* node = doc.GetRoot();

  if ( node == NULL )
    throw xml::missing_node("level");

  return load_level( pool, node );
} // level_file_xml_reader::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a node of type "level".
 * \param pool The pool of item classes in which we take the item classes.
 * \param node The node to parse.
 */
bf::gui_level* bf::level_file_xml_reader::load_level
( const item_class_pool& pool, const wxXmlNode* node ) const
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
      load_layers( pool, *lvl, node->GetChildren() );
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
 */
void bf::level_file_xml_reader::load_layers
( const item_class_pool& pool, gui_level& lvl, const wxXmlNode* node ) const
{
  for ( node=xml::reader_tool::skip_comments(node); node!=NULL;
        node=xml::reader_tool::skip_comments(node->GetNext()) )
    if ( node->GetName() == wxT("layer") )
      load_layer(pool, lvl, node);
    else
      claw::logger << claw::log_warning << "Ignored node '"
                   << wx_to_std_string(node->GetName()) << "'" << std::endl;
} // level_file_xml_reader::load_layers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load one layer.
 * \param pool The pool of item classes in which we take the item classes.
 * \param lvl The level in which we store the layer.
 * \param node The node to parse.
 */
void bf::level_file_xml_reader::load_layer
( const item_class_pool& pool, gui_level& lvl, const wxXmlNode* node ) const
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

  load_layer_content( pool, lay, node->GetChildren() );
} // level_file_xml_reader::load_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the content of a layer.
 * \param pool The pool of item classes in which we take the item classes.
 * \param lay The layer in which we store the instances.
 * \param node The node to parse.
 */
void bf::level_file_xml_reader::load_layer_content
( const item_class_pool& pool, layer& lay, const wxXmlNode* node ) const
{
  node=xml::reader_tool::skip_comments(node);

  if ( node != NULL )
    {
      if ( node->GetName() == wxT("items") )
        load_items( pool, lay, node->GetChildren() );

      node=xml::reader_tool::skip_comments(node->GetNext());

      if ( node != NULL )
        if ( node->GetName() == wxT("priority") )
          load_priorities( lay, node->GetChildren() );
    }
} // level_file_xml_reader::load_layer_content()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the items of a layer.
 * \param pool The pool of item classes in which we take the item classes.
 * \param lay The layer in which we store the instances.
 * \param node The node to parse.
 */
void bf::level_file_xml_reader::load_items
( const item_class_pool& pool, layer& lay, const wxXmlNode* node ) const
{
  for ( node=xml::reader_tool::skip_comments(node); node!=NULL;
        node=xml::reader_tool::skip_comments(node->GetNext()) )
    if ( node->GetName() == wxT("item") )
      load_item(pool, lay, node);
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
 * \param pool The pool of item classes in which we take the item classes.
 * \param lay The layer in which we store the item.
 * \param node The node to parse.
 */
void bf::level_file_xml_reader::load_item
( const item_class_pool& pool, layer& lay, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node->GetName() == wxT("item") );

  xml::item_instance_node item_node;
  item_instance* item = item_node.read( pool, node );

  if ( item != NULL )
    lay.add_item(item);
} // level_file_xml_reader::load_item()
