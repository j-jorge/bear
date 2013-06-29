/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::node_parser_file class.
 * \author Julien Jorge
 */
#include "engine/script/node_parser/node_parser_file.hpp"

#include "engine/script/node_parser/node_parser_call_entry.hpp"
#include "engine/script/script_grammar.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a node of type file.
 * \param seq The sequence in which the calls are inserted.
 * \param node Node to parse.
 * \param t The translator to use to translate the texts.
 */
void bear::engine::node_parser_file::parse_node
( call_sequence& seq, const tree_node& node, translator t ) const
{
  node_parser_call_entry call;

  if ( node.value.id() == script_grammar::id_call_entry )
    call.parse_node(seq, node, t);
  else // node.value.id() == script_grammar::id_file
    for (std::size_t i=0; i!=node.children.size(); ++i)
      call.parse_node(seq, node.children[i], t);
} // node_parser_file::parse_node()
