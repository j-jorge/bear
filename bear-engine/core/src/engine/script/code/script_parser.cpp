/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::script_parser class.
 * \author Julien Jorge
 */
#include "engine/script/script_parser.hpp"

#include "engine/script/call_sequence.hpp"
#include "engine/script/script_grammar.hpp"
#include "engine/script/node_parser/node_parser_file.hpp"

#include "engine/game.hpp"
#include "engine/resource_pool.hpp"

#include <sstream>
#include <map>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a script.
 * \param seq The call_sequence in which the result is stored.
 * \param path The path of the script resource.
 */
bool bear::engine::script_parser::run
( call_sequence& seq, const std::string& path )
{
  std::stringstream file_data;

  bool ok;

  if ( !resource_pool::get_instance().exists(path) )
    {
      claw::logger << claw::log_error << "Can't find file '" << path << "'."
                   << std::endl;
      ok = false;
    }
  else
    {
      resource_pool::get_instance().get_file(path, file_data);
      ok = run( seq, file_data.str().c_str(), file_data.str().size() );
    }

  return ok;
} // model_compiler::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a script.
 * \param seq The call_sequence in which the result is stored.
 * \param in_file The script to parse.
 */
bool bear::engine::script_parser::run
( call_sequence& seq, const std::istream& in_file )
{
  std::stringstream file_data;

  file_data << in_file.rdbuf();

  bool ok = run( seq, file_data.str().c_str(), file_data.str().size() );

  return ok;
} // model_compiler::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a script.
 * \param seq The call_sequence in which the result is stored.
 * \param file_data The content of the file to compile.
 * \param file_size Size of the \a file_data buffer.
 */
bool bear::engine::script_parser::run
( call_sequence& seq, const char* file_data, unsigned int file_size )
{
  bool ok;

  boost::spirit::classic::tree_parse_info<iterator, node_factory> info;
  script_grammar grammar;
  iterator begin(file_data, file_data + file_size, "script"), end;

  info = boost::spirit::classic::ast_parse<node_factory>
    ( begin, end, grammar,
      boost::spirit::classic::comment_p("/*", "*/")
      | boost::spirit::classic::space_p );

  ok = info.match;

  scan_tree( seq, info.trees[0] );

  return ok;
} // model_compiler::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the tree of the file.
 * \param seq The file to write in.
 * \param node The root of the tree to compile.
 */
void bear::engine::script_parser::scan_tree
( call_sequence& seq, const tree_node& node )
{
  node_parser_file file;

  file.parse_node( seq, node, game::get_instance().get_translator() );
} // bear::engine::script_parser::parse_tree()
