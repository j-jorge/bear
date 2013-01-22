/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::node_parser_argument class.
 * \author Julien Jorge
 */
#include "engine/script/node_parser/node_parser_argument.hpp"

#include "engine/i18n/translator.hpp"
#include "engine/script/script_grammar.hpp"

#include <claw/string_algorithm.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a node of type argument.
 * \param val The value of the argument.
 * \param node Node to parse.
 * \param t The translator to use to translate the texts.
 */
void bear::engine::node_parser_argument::parse_node
( std::string& val, const tree_node& node, translator t ) const
{
  val = std::string( node.value.begin(), node.value.end() );

  if ( node.value.id() == script_grammar::id_string )
    {
      std::string tmp( t.get( val ) );
      val.clear();

      claw::text::c_escape
        ( tmp.begin(), tmp.end(), std::inserter(val, val.end()) );
    }
} // node_parser_argument::parse_node()
