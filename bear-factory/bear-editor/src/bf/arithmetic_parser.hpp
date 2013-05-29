/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A parser for simple arithmetic expressions.
 * \author Julien Jorge
 */
#ifndef __BF_ARITHMETIC_PARSER_HPP__
#define __BF_ARITHMETIC_PARSER_HPP__

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>

#include <string>

namespace bf
{
  /**
   * \brief A parser for simple arithmetic expressions.
   * \author Julien Jorge
   */
  class arithmetic_parser
  {
  private:
    typedef boost::spirit::classic::position_iterator<const char*> iterator;
    typedef
    boost::spirit::classic::node_iter_data_factory<iterator> node_factory;
    typedef
    boost::spirit::classic::tree_match<iterator, node_factory> tree_match;
    typedef tree_match::node_t tree_node;

  public:
    bool evaluate( double& result, std::string expr ) const;
 
  private:
    bool evaluate_tree( double& result, tree_node const& tree ) const;
    bool evaluate_operator
    ( double& result, char op, tree_node const& left_tree,
      tree_node const& right_tree ) const;

    bool parse_real( double& result, std::string s ) const;

  }; // class arithmetic_parser
} // namespace bf

#endif // __BF_ARITHMETIC_PARSER_HPP__
