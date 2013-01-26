/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Compile the node of the "xcf_declaration" rule.
 * \author Julien Jorge
 */
#ifndef __SDC_NODE_PARSER_XCF_ENTRY_HPP__
#define __SDC_NODE_PARSER_XCF_ENTRY_HPP__

#include "node_parser/node_parser.hpp"

namespace sdc
{
  class spritedesc;
  class xcf_map;

  /**
   * \brief Compile the node of the "xcf_declaration" rule.
   * \author Julien Jorge
   */
  class node_parser_xcf_entry:
    public node_parser
  {
  public:
    void parse_node
    ( xcf_map& xcf, spritedesc& desc, const tree_node& node ) const;

  }; // class node_parser_xcf_entry
} // namespace sdc

#endif // __SDC_NODE_PARSER_XCF_ENTRY_HPP__
