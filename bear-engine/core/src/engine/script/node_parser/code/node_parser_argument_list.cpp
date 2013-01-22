/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::node_parser_argument_list class.
 * \author Julien Jorge
 */
#include "engine/script/node_parser/node_parser_argument_list.hpp"

#include "engine/script/method_call.hpp"
#include "engine/script/script_grammar.hpp"
#include "engine/script/node_parser/node_parser_argument.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a node of type argument_list.
 * \param call The method call for which we set the arguments.
 * \param node Node to parse.
 * \param t The translator to use to translate the texts.
 */
void bear::engine::node_parser_argument_list::parse_node
( method_call& call, const tree_node& node, translator t ) const
{
  std::vector<std::string> args;
  node_parser_argument parser;
  std::string val;

  if ( node.value.id() == script_grammar::id_argument_list )
    {
      for (std::size_t i=0; i!=node.children.size(); ++i)
        {
          parser.parse_node(val, node.children[i], t);
          args.push_back(val);
        }
    }
  else
    {
      parser.parse_node(val, node, t);
      args.push_back(val);
    }

  call.set_arguments(args);
} // node_parser_argument_list::parse_node()
