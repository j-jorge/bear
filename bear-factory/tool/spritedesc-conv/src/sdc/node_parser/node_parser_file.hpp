/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Compile the node of the "file" rule.
 * \author Julien Jorge
 */
#ifndef __SDC_NODE_PARSER_FILE_HPP__
#define __SDC_NODE_PARSER_FILE_HPP__

#include "node_parser/node_parser.hpp"

#include "spritedesc.hpp"

namespace sdc
{
  class xcf_map;

  /**
   * \brief Compile the node of the "file" rule.
   * \author Julien Jorge
   */
  class node_parser_file:
    public node_parser
  {
  public:
    void parse_node
    ( xcf_map& xcf, std::list<spritedesc>& desc, const tree_node& node ) const;

  private:
    void process_sprite_sheet
    ( xcf_map& xcf, std::list<spritedesc>& desc, const tree_node& node ) const;

  }; // class node_parser_file
} // namespace sdc

#endif // __SDC_NODE_PARSER_FILE_HPP__
