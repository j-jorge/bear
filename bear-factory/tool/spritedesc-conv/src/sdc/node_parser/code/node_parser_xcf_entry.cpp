/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the sdc::node_parser_xcf_entry class.
 * \author Julien Jorge
 */
#include "node_parser/node_parser_xcf_entry.hpp"

#include "spritedesc.hpp"
#include "xcf_map.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a node of type file.
 * \param xcf The description of the content of the xcf.
 * \param desc The structure receiving the result of the parsing.
 * \param node Node to parse.
 */
void sdc::node_parser_xcf_entry::parse_node
( xcf_map& xcf, spritedesc& desc, const tree_node& node ) const
{
  CLAW_PRECOND( node.children.size() == 2 );

  const std::string id
    ( node.children[0].value.begin(), node.children[0].value.end() );
  const std::string name
    ( node.children[1].value.begin(), node.children[1].value.end() );
  
  xcf.load( name );

  desc.xcf[ id ] = name;
} // node_parser_xcf_entry::parse_node()
