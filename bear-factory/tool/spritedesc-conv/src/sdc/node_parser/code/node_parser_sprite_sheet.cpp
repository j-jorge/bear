/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the sdc::node_parser_sprite_sheet class.
 * \author Julien Jorge
 */
#include "node_parser/node_parser_sprite_sheet.hpp"

#include "node_parser/node_parser_xcf_entry.hpp"
#include "node_parser/node_parser_sprite_declaration.hpp"
#include "node_parser/node_parser_layer_to_sprite.hpp"

#include "grammar.hpp"

#include "spritedesc.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a node describing a sprite sheet.
 * \param xcf The description of the content of the xcf.
 * \param desc The structure receiving the result of the parsing.
 * \param node Node to parse.
 */
void sdc::node_parser_sprite_sheet::parse_node
( xcf_map& xcf, spritedesc& desc, const tree_node& node ) const
{
  CLAW_PRECOND( node.children.size() > 4 );

  parse_name( desc, node.children[0] );
  parse_width( desc, node.children[1] );
  parse_height( desc, node.children[2] );
  
  std::size_t i(3);

  if ( parse_margin( desc, node.children[i] ) )
    ++i;

  if ( parse_order( desc, node.children[i] ) )
    ++i;

  while ( (i != node.children.size())
          && ( node.children[i].value.id() == grammar::id_xcf_declaration ) )
    {
      node_parser_xcf_entry call;
      call.parse_node( xcf, desc, node.children[i] );
      ++i;
    }

  while ( i != node.children.size() )
    {
      if ( node.children[i].value.id() == grammar::id_sprite_declaration )
        {
          node_parser_sprite_declaration call;
          call.parse_node( xcf, desc, node.children[i] );
        }
      else // id == grammar::id_layer_to_sprite
        {
          node_parser_layer_to_sprite call;
          call.parse_node( xcf, desc, node.children[i] );
        }

      ++i;
    }

  check_xcf_usage( desc );
} // node_parser_sprite_sheet::parse_node()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse the name of the sprite sheet.
 * \param desc The structure receiving the result of the parsing.
 * \param node Node to parse.
 */
void sdc::node_parser_sprite_sheet::parse_name
( spritedesc& desc, const tree_node& node ) const
{
  desc.output_name = std::string( node.value.begin(), node.value.end() );
} // node_parser_sprite_sheet::parse_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse the width of the sprite sheet.
 * \param desc The structure receiving the result of the parsing.
 * \param node Node to parse.
 */
void sdc::node_parser_sprite_sheet::parse_width
( spritedesc& desc, const tree_node& node ) const
{
  std::istringstream iss( std::string( node.value.begin(), node.value.end() ) );
  iss >> desc.width;
} // node_parser_sprite_sheet::parse_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse the height of the sprite sheet.
 * \param desc The structure receiving the result of the parsing.
 * \param node Node to parse.
 */
void sdc::node_parser_sprite_sheet::parse_height
( spritedesc& desc, const tree_node& node ) const
{
  std::istringstream iss( std::string( node.value.begin(), node.value.end() ) );
  iss >> desc.height;
} // node_parser_sprite_sheet::parse_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse the margin between the sprites.
 * \param desc The structure receiving the result of the parsing.
 * \param node Node to parse.
 * \return true if the margin has been read.
 */
bool sdc::node_parser_sprite_sheet::parse_margin
( spritedesc& desc, const tree_node& node ) const
{
  if ( node.value.id() == grammar::id_margin )
    {
      std::istringstream iss
        ( std::string( node.value.begin(), node.value.end() ) );
      iss >> desc.margin;
      return true;
    }
  else
    {
      desc.margin = 1;
      return false;
    }
} // node_parser_sprite_sheet::parse_margin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse the heuristic to use to place the sprites.
 * \param desc The structure receiving the result of the parsing.
 * \param node Node to parse.
 * \return true if the margin has been read.
 */
bool sdc::node_parser_sprite_sheet::parse_order
( spritedesc& desc, const tree_node& node ) const
{
  desc.order = decreasing_area;

  if ( node.value.id() == grammar::id_string )
    {
      const std::string order_string( node.value.begin(), node.value.end() );

      if ( order_string == "none" )
        desc.order = declaration_order;
      else if ( order_string == "area" )
        desc.order = decreasing_area;
      else if ( order_string == "height" )
        desc.order = decreasing_height;
      else
        std::cerr << "Unknown order type '" << order_string << "'."
                  << std::endl;

      return true;
    }
  else
    return false;
} // node_parser_sprite_sheet::parse_order()

/*----------------------------------------------------------------------------*/
/**
 * \brief Displays a warning for each XCF file declared but not used in the
 *        sprite sheet.
 * \param desc The sprite sheet to check.
 */
void sdc::node_parser_sprite_sheet::check_xcf_usage
( const spritedesc& desc ) const
{
  spritedesc::id_to_file_map xcf_files = desc.xcf;
  
  for ( spritedesc::const_sprite_iterator it = desc.sprite_begin();
        it != desc.sprite_end(); ++it )
    xcf_files.erase( it->xcf_id );

  for ( spritedesc::id_to_file_map::const_iterator it = xcf_files.begin();
        it != xcf_files.end(); ++it )
    std::clog << "warning: XCF file '" << it->second << "' identified as '"
              << it->first << "' is never used in sprite sheet '"
              << desc.output_name << "'." << std::endl;
} // node_parser_sprite_sheet::check_xcf_usage()
