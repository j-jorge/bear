/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::node_parser_call_entry class.
 * \author Julien Jorge
 */
#include "engine/script/node_parser/node_parser_call_entry.hpp"

#include "engine/script/node_parser/node_parser_call.hpp"
#include "engine/script/node_parser/node_parser_call_group.hpp"

#include "engine/script/call_sequence.hpp"
#include "engine/script/script_grammar.hpp"

#include <claw/assert.hpp>
#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a node of type call.
 * \param seq The sequence in which the calls are inserted.
 * \param node Node to parse.
 * \param t The translator to use to translate the texts.
 */
void bear::engine::node_parser_call_entry::parse_node
( call_sequence& seq, const tree_node& node, translator t ) const
{
  CLAW_PRECOND( node.children.size() == 2 );

  universe::time_type date;
  std::istringstream iss
    ( std::string( node.children[0].value.begin(),
                   node.children[0].value.end() ) );

  iss >> date;

  if ( *node.children[0].value.begin() == '+' )
    date += seq.get_last_date();

  if ( node.children[1].value.id() == script_grammar::id_call )
    {
      node_parser_call call;
      call.parse_node(seq, node.children[1], date, t);
    }
  else // node.value.id() == script_grammar::id_call_group
    {
      node_parser_call_group call;
      call.parse_node(seq, node.children[1], date, t);
    }
} // node_parser_call_entry::parse_node()
