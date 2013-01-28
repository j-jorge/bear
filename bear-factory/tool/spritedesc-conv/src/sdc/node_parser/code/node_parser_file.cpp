/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the sdc::node_parser_file class.
 * \author Julien Jorge
 */
#include "node_parser/node_parser_file.hpp"

#include "node_parser/node_parser_sprite_sheet.hpp"

#include "grammar.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a node of type file.
 * \param xcf The description of the content of the xcf.
 * \param desc The list of the sprite descriptions read in the node.
 * \param node Node to parse.
 */
void sdc::node_parser_file::parse_node
( xcf_map& xcf, std::list<spritedesc>& desc, const tree_node& node ) const
{
  if ( node.value.id() == grammar::id_sprite_sheet )
    process_sprite_sheet( xcf, desc, node );
  else
    for ( std::size_t i=0; i!=node.children.size(); ++i )
      process_sprite_sheet( xcf, desc, node.children[i] );
} // node_parser_file::parse_node()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a node of type sprite_sheet.
 * \param xcf The description of the content of the xcf.
 * \param desc The list of the sprite descriptions read in the node.
 * \param node Node to parse.
 */
void sdc::node_parser_file::process_sprite_sheet
( xcf_map& xcf, std::list<spritedesc>& desc, const tree_node& node ) const
{
  node_parser_sprite_sheet parser;
  spritedesc result;

  parser.parse_node( xcf, result, node );

  bool duplicate(false);

  for ( std::list<spritedesc>::const_iterator it=desc.begin();
        !duplicate && (it!=desc.end());
        ++it )
    duplicate = (it->output_name == result.output_name);

  if ( duplicate )
    std::cerr << "Duplicate output: '" << result.output_name
              << "' ignored." << std::endl;
  else
    desc.push_back( result );
} // node_parser_file::process_sprite_sheet()
