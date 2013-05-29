/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A parser for simple arithmetic expressions.
 * \author Julien Jorge
 */
#include "bf/arithmetic_parser.hpp"

#include "bf/arithmetic_grammar.hpp"

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>
#include <boost/lexical_cast.hpp>

#include <claw/assert.hpp>

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * 
 */
bool bf::arithmetic_parser::evaluate( double& result, std::string expr ) const
{
  arithmetic_grammar grammar;
  iterator begin( expr.c_str(), expr.c_str() + expr.size() );
  iterator end;

  boost::spirit::classic::tree_parse_info<iterator, node_factory> info;

  info =
    boost::spirit::classic::ast_parse<node_factory>
    ( begin, end, grammar, boost::spirit::classic::space_p );

  if ( !info.match )
    return false;
  else
    return evaluate_tree( result, info.trees[0] );
} // arithmetic_parser::evaluate()
 
bool bf::arithmetic_parser::evaluate_tree
( double& result, tree_node const& tree ) const
{
  switch (tree.value.id().to_long())
    {
    case arithmetic_grammar::id_real:
      return parse_real
        ( result, std::string( tree.value.begin(), tree.value.end() ) );
      
    case arithmetic_grammar::id_expression:
    case arithmetic_grammar::id_term:
    case arithmetic_grammar::id_factor:
      return evaluate_operator
        ( result, *tree.value.begin(), *tree.children.begin(),
          *(tree.children.begin() + 1) );

    default:
      CLAW_FAIL( "unknown tree identifier" );
    }
 
  return false;
} // arithmetic_parser::evaluate_tree()

bool bf::arithmetic_parser::evaluate_operator
( double& result, char op, tree_node const& left_tree,
  tree_node const& right_tree ) const
{
  double left;
  double right;

  if( !evaluate_tree( left, left_tree )
      || !evaluate_tree( right, right_tree ) )
    return false;

  switch( op )
    {
    case '+':
      result = left + right;
      return true;
    case '-':
      result = left - right;
      return true;
    case '*':
      result = left * right;
      return true;
    case '/':
      result = left / right;
      return true;
    default:
      CLAW_FAIL( std::string( "unknown operand: " ) + op );
    }

  return false;
} // arithmetic_parser::evaluate_operator()

bool bf::arithmetic_parser::parse_real( double& result, std::string s ) const
{
  try
    {
      result = boost::lexical_cast<double>(s);
      return true;
    }
  catch( const boost::bad_lexical_cast& e )
    {
      return false;
    }
} // arithmetic_parser::parse_real()
