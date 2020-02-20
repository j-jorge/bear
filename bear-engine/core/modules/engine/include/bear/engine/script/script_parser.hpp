/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The parser for the script files.
 * \author Julien Jorge
 */
#ifndef __ENGINE_SCRIPT_PARSER_HPP__
#define __ENGINE_SCRIPT_PARSER_HPP__

#include "bear/engine/class_export.hpp"

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>

#include <istream>

namespace bear
{
  namespace engine
  {
    class call_sequence;

    /**
     * \brief The parser for the script files.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT script_parser
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
      bool run( call_sequence& seq, const std::string& path );
      bool run( call_sequence& seq, const std::istream& in_file );
      bool run
      ( call_sequence& seq, const char* file_data, unsigned int file_size );

    private:
      void scan_tree( call_sequence& seq, const tree_node& node );

    }; // class script_parser
  } // namespace engine
} // namespace bear

#endif // __ENGINE_SCRIPT_PARSER_HPP__
