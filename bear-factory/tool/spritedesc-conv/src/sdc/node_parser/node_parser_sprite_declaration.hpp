/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Compile the node of the "sprite_declaration" rule.
 * \author Julien Jorge
 */
#ifndef __SDC_NODE_PARSER_SPRITE_DECLARATION_HPP__
#define __SDC_NODE_PARSER_SPRITE_DECLARATION_HPP__

#include "node_parser/node_parser_sprite_description_base.hpp"

namespace sdc
{
  /**
   * \brief Compile the node of the "sprite_declaration" rule.
   * \author Julien Jorge
   */
  class node_parser_sprite_declaration:
    public node_parser_sprite_description_base
  {
  public:
    void parse_node
    ( const xcf_map& xcf, spritedesc& s, const tree_node& node ) const;

  private:
    void get_sprite_name
    ( spritedesc::sprite& s, const tree_node& node ) const;
    void get_image_id
    ( spritedesc::sprite& s, const tree_node& node ) const;
    void get_layers_and_size
    ( const xcf_info& image, spritedesc::sprite& s, const tree_node& size_node,
      const tree_node& layer_list_node ) const;

    std::list<layer_info> get_layers
    ( const xcf_info& image, const tree_node& layer_list_node ) const;
    void add_layers
    ( const xcf_info& image, std::list<layer_info>& result,
      const tree_node& node ) const;
    void add_layers_glob
    ( const xcf_info& image, std::list<layer_info>& result,
      const tree_node& node ) const;
    void exclude_layers
    ( const xcf_info& image, std::list<layer_info>& result,
      const tree_node& node ) const;
    void add_single_layer
    ( const xcf_info& image, std::list<layer_info>& result,
      const tree_node& node ) const;

    void compute_source_box( spritedesc::sprite& s ) const;

    void get_layer_info
    ( const xcf_info& xcf, layer_info& layer, const tree_node& node ) const;

  }; // class node_parser_sprite_declaration
} // namespace sdc

#endif // __SDC_NODE_PARSER_SPRITE_DECLARATION_HPP__
