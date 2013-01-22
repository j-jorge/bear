/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Compile the node of the "sprite_declaration" rule.
 * \author Julien Jorge
 */
#ifndef __SDC_NODE_PARSER_LAYER_TO_SPRITE_HPP__
#define __SDC_NODE_PARSER_LAYER_TO_SPRITE_HPP__

#include "node_parser/node_parser_sprite_description_base.hpp"

namespace sdc
{
  /**
   * \brief Compile the node of the "layer_to_sprite" rule.
   * \author Julien Jorge
   */
  class node_parser_layer_to_sprite:
    public node_parser_sprite_description_base
  {
  public:
    void parse_node
    ( const xcf_map& xcf, spritedesc& s, const tree_node& node ) const;

  }; // class node_parser_layer_to_sprite
} // namespace sdc

#endif // __SDC_NODE_PARSER_LAYER_TO_SPRITE_HPP__
