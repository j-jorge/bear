/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Compile the node of the "image_size" rule.
 * \author Julien Jorge
 */
#ifndef __SDC_NODE_PARSER_IMAGE_SIZE_HPP__
#define __SDC_NODE_PARSER_IMAGE_SIZE_HPP__

#include "node_parser/node_parser.hpp"

#include "spritedesc.hpp"

namespace sdc
{
  /**
   * \brief Compile the node of the "image_size" rule.
   * \author Julien Jorge
   */
  class node_parser_image_size:
    public node_parser
  {
  public:
    void parse_node
    ( const xcf_info& xcf, spritedesc::sprite& s, const tree_node& node ) const;

  }; // class node_parser_image_size
} // namespace sdc

#endif // __SDC_NODE_PARSER_IMAGE_SIZE_HPP__
