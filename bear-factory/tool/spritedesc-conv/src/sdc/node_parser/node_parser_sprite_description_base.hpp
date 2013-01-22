/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Base class for the nodes describing a sprite.
 * \author Julien Jorge
 */
#ifndef __SDC_NODE_PARSER_SPRITE_DESCRIPTION_BASE_HPP__
#define __SDC_NODE_PARSER_SPRITE_DESCRIPTION_BASE_HPP__

#include "node_parser/node_parser.hpp"

#include "xcf_map.hpp"
#include "spritedesc.hpp"

namespace sdc
{
  /**
   * \brief Base class for the nodes describing a sprite.
   * \author Julien Jorge
   */
  class node_parser_sprite_description_base:
    public node_parser
  {
  protected:
    void apply_result_box_ratio
    ( spritedesc::sprite& s, const tree_node& node ) const;

    void crop_sprite_to_image_bounds
    ( const xcf_info& image, spritedesc::sprite& s ) const;

    void get_xcf_from_id
    ( const xcf_map& xcf, const spritedesc& desc, xcf_info& info,
      const std::string& xcf_id ) const;

  }; // class node_parser_sprite_description_base
} // namespace sdc

#endif // __SDC_NODE_PARSER_SPRITE_DESCRIPTION_BASE_HPP__
