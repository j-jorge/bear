/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::node_parser_call_group class.
 * \author Julien Jorge
 */
#include "engine/script/node_parser/node_parser_call_group.hpp"

#include "engine/script/node_parser/node_parser_call.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a node of type call_group.
 * \param seq The sequence in which the calls are inserted.
 * \param node Node to parse.
 * \param date The date of the call.
 * \param t The translator to use to translate the texts.
 */
void bear::engine::node_parser_call_group::parse_node
( call_sequence& seq, const tree_node& node, universe::time_type date,
  translator t ) const
{
  CLAW_PRECOND( node.children.size() >= 1 );

  node_parser_call call;

  for ( std::size_t i=0; i!=node.children.size(); ++i )
    call.parse_node( seq, node.children[i], date, t );
} // node_parser_call_group::parse_node()
