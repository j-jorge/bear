/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The parser for the spritedesc files.
 * \author Julien Jorge
 */
#ifndef __SDC_PARSER_HPP__
#define __SDC_PARSER_HPP__

#include "spritedesc.hpp"

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>

#include <istream>

namespace sdc
{
  class xcf_map;

  /**
   * \brief The parser for the .spritedesc files.
   * \author Julien Jorge
   */
  class parser
  {
  public:
    typedef boost::spirit::classic::position_iterator<const char*> iterator;
    typedef
      boost::spirit::classic::node_iter_data_factory<iterator> node_factory;
    typedef
      boost::spirit::classic::tree_match<iterator, node_factory> tree_match;
    typedef tree_match::node_t tree_node;

  private:
    typedef boost::spirit::classic::scanner<iterator> scanner;

  public:
    bool run
    ( xcf_map& xcf, std::list<spritedesc>& desc, const std::string& path );
    bool run
      ( xcf_map& xcf, std::list<spritedesc>& desc, const char* file_data,
        unsigned int file_size );

  private:
    void scan_tree
    ( xcf_map& xcf, std::list<spritedesc>& desc, const tree_node& node ) const;

  }; // class parser
} // namespace sdc

#endif // __SDC_PARSER_HPP__

