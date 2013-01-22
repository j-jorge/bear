/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the sdc::parser class.
 * \author Julien Jorge
 */
#include "parser.hpp"

#include "spritedesc.hpp"
#include "grammar.hpp"

#include "node_parser/node_parser_file.hpp"

#include <fstream>
#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse the description of the sprites.
 * \param xcf The map associating the name of the xcf with their descriptions.
 * \param desc The result of the parsing.
 * \param path The path of the file to read.
 */
bool sdc::parser::run
( const xcf_map& xcf, std::list<spritedesc>& desc, const std::string& path )
{
  std::stringstream file_data;

  bool ok;
  std::ifstream f( path.c_str() );

  if ( !f )
    {
      std::cerr << "Can't find file '" << path << "'." << std::endl;
      ok = false;
    }
  else
    {
      file_data << f.rdbuf();
      ok = run( xcf, desc, file_data.str().c_str(), file_data.str().size() );
    }

  return ok;
} // model_compiler::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse the description of the sprites.
 * \param xcf The map associating the name of the xcf with their descriptions.
 * \param desc The result of the parsing.
 * \param file_data The content of the file to compile.
 * \param file_size Size of the \a file_data buffer.
 */
bool sdc::parser::run
( const xcf_map& xcf, std::list<spritedesc>& desc, const char* file_data,
  unsigned int file_size )
{
  bool ok;

  boost::spirit::classic::tree_parse_info<iterator, node_factory> info;
  grammar grammar;
  iterator begin(file_data, file_data + file_size, "script"), end;

  info = boost::spirit::classic::ast_parse<node_factory>
    ( begin, end, grammar,
      boost::spirit::classic::comment_p("/*", "*/")
      | boost::spirit::classic::space_p );

  ok = info.match;

  scan_tree( xcf, desc, info.trees[0] );

  return ok;
} // model_compiler::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the tree of the file.
 * \param xcf The map associating the name of the xcf with their descriptions.
 * \param desc The result of the parsing.
 * \param node The root of the tree to compile.
 */
void sdc::parser::scan_tree
( const xcf_map& xcf, std::list<spritedesc>& desc, const tree_node& node ) const
{
  node_parser_file file;

  file.parse_node( xcf, desc, node );
} // parser::parse_tree()
