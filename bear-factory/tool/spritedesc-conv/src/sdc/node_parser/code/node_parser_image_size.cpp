/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the sdc::node_parser_image_size class.
 * \author Julien Jorge
 */
#include "node_parser/node_parser_image_size.hpp"

#include "spritedesc.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a node of type image_size.
 * \param s The structure receiving the result of the parsing.
 * \param node Node to parse.
 */
void sdc::node_parser_image_size::parse_node
( spritedesc::sprite& s, const tree_node& node ) const
{
  const std::string input( node.value.begin(), node.value.end() );
  std::istringstream iss( input );
  double ratio;

  if ( iss >> ratio )
    {
      s.result_box.width = s.source_box.width * ratio;
      s.result_box.height = s.source_box.height * ratio;
    }
  else
    std::cerr << "invalid ratio '" << input
              << "' for sprite '" << s.name << "'" << std::endl;
} // node_parser_image_size::parse_node()
