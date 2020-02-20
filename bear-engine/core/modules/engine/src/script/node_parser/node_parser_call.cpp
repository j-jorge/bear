/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::node_parser_call class.
 * \author Julien Jorge
 */
#include "bear/engine/script/node_parser/node_parser_call.hpp"

#include "bear/engine/script/node_parser/node_parser_argument_list.hpp"

#include "bear/engine/script/call_sequence.hpp"
#include "bear/engine/script/method_call.hpp"

#include <claw/assert.hpp>
#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a node of type call.
 * \param seq The sequence in which the calls are inserted.
 * \param node Node to parse.
 * \param date The date of the call.
 * \param t The translator to use to translate the texts.
 */
void bear::engine::node_parser_call::parse_node
( call_sequence& seq, const tree_node& node, universe::time_type date,
  translator t ) const
{
  CLAW_PRECOND( node.children.size() >= 2 );
  CLAW_PRECOND( node.children.size() <= 3 );

  method_call call;

  call.set_actor_name
    ( std::string( node.children[0].value.begin(),
                   node.children[0].value.end() ) );
  call.set_method_name
    ( std::string( node.children[1].value.begin(),
                   node.children[1].value.end() ) );

  if ( node.children.size() > 2 )
    {
      node_parser_argument_list parser;
      parser.parse_node(call, node.children[2], t);
    }

  seq.add_call(date, call);
} // node_parser_call::parse_node()
